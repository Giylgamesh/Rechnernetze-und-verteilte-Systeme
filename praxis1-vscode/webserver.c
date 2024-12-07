//#include "server_sructs.h"
//#include "helper_functions.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

// #define MYPORT "3490"    // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

/* 
Deruves a sockaddr_in strucure from the provided host and port information

@param host: the host (IP address or hostname) to be resolved ino a network address
@param port: the port number to be converted into network byte order

@return: a sockaddr_in structure representing the network address derived from the host and port
*/
static struct sockaddr_in derive_sockaddr(const char* host, const char* port){
    struct addrinfo hints = {
        .ai_family = AF_INET,
    };
    struct addrinfo *result_info;

    //resolve the host (IP address or hostname) into a list of possible addresses
    int returncode = getaddrinfo(host, port, &hints, &result_info);
    if (returncode != 0){
        fprintf(stderr, "Error parsing host/port information %s:%s\n", host, port);
        exit(EXIT_FAILURE);
    }

    // copy the sockaddr_in structure from the first address in the list
    struct sockaddr_in result = *((struct sockaddr_in*) result_info->ai_addr);

    // free allocated memory for result_info
    freeaddrinfo(result_info);
    return result;
}

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// debug flag
int debug = 1;

//-----------------MAIN FUNCTION---------------------
int main(int argc, char *argv[])
{
    // ---DEBUG--- 
    //print out the command line arguments (number of arguments and the arguments themselves)
    if (debug == 1)
    {
        printf("----------DEBUG------------- main() called\n");
        printf("number of arguments (argc): %d\n", argc);
        for (int i = 0; i < argc; i++)
        {
            printf("argv[%d]: %s\n", i, argv[i]);
        }
        printf("----------DEBUG END-------------main() called\n");
    } //---DEBUG END---


    int sockfd; // socket file descriptor
    int new_fd;
    int status; 

    struct sockaddr_in my_addr; // server address information
    struct addrinfo hints, *servinfo, *P; // serverinfo (previously res) 
    struct sockaddr_storage their_addr; // client address information
    socklen_t addr_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    // ------check for command line arguments to get the host and port
    const char *host = argv[1];
    const char *port = argv[2];
    // check if the host and port are provided)
    if (argc < 3) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    } else {
        host = argv[1]; // get host from command-line argument
        port = argv[2]; // get port from command-line argument
    }
    printf("HOST: %s \n PORT IN USE: %s \n", host, port); 

    // using the given derive_sockaddr function to get the address
    my_addr = derive_sockaddr(host, port); 


    memset(&hints, 0, sizeof hints); // zeroing out the memory so that the struct is empty
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC can be either AF_INET or AF_INET6 (IPv4 or IPv6)
    hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM is TCP , SOCK_DGRAM is UDP
    hints.ai_flags = AI_PASSIVE; // assign the address of my local host to the socket structures
    hints.ai_protocol = 0; // any protocol


    // getaddrinfo() returns a linked list of addrinfo structures
    status = getaddrinfo(host, port, &hints, &servinfo); // get the address information of the host and port
    if (status != 0) { // if the status is not 0, then there is an error
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }


    // -------------CREATE SOCKET----------------
    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd <  0) {
        close(sockfd);
        perror("ERROR while creating a socket"); // fprintf(stderr, "socket: %s\n", strerror(errno));
        return 2; // specific error
    }


    // bind it to the port we passed in to getaddrinfo():
    // bind() returns 0 of successfull, -1 for errors
    if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        close(sockfd);
        perror("ERROR while binding a socket"); // fprintf(stderr, "bind: %s\n", strerror(errno));
        return 2; // specific error
    }

    // listen for incoming connections on the socket sockfd
    int backlog = 3;  // maximum number of connections waiting in queue
    if (listen(sockfd, backlog) < 0) {
        close(sockfd);
        perror("ERROR while listening for incoming connections");
        return 2; // specific error
    }
   
    freeaddrinfo(servinfo);


    char buffer[1024]; // buffer to store the incoming data
    
    if (debug == 1)
    {
        printf("Size of buffer: %ld\n", sizeof(buffer));
    }
    
    memset(buffer, 0, sizeof(buffer)); // zeroing out the memory so that the buffer is empty
    int counter = 0; // counter for while loop
    while (1) {

        if (debug ==1){
            counter++;
            printf("LOOP COUNTER %d\n", counter);

        }
        // accept a connection attempt to a speciic socket
        /*addr will usually be a pointer to a local struct sockaddr_storage. 
        This is where the information about the incoming connection will go 
        (and with it you can determine which host is calling you from which port)*/
        addr_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size); // only relevant for TCP protocol, becouse we build a continues data stream
        if (new_fd < 0) {
            perror("ERROR while accepting a connection");
            continue;
        }   
    
        // receive data from the socket
        
        if (debug == 1)
        {
            printf("----------DEBUG------------- main() called\n");
            printf("Address of buffer: %p\n", &buffer);
            printf("content of buffer: %s\n", buffer);
        }
        
        int number_of_bytes_received = recv(new_fd, buffer, sizeof(buffer) - 1, 0); // recv() returns the number of bytes received, or -1 if an error occurred or 0 if the connection is closed
        if (number_of_bytes_received < 0) { // error occured while receiving data from the client 
            perror("ERROR while receiving data"); 
            close(new_fd); // close connection, as error occured
            continue;
        } else if (number_of_bytes_received == 0) {
            printf("connection closed on other end!\n");
            close(new_fd); // close the connection
            continue;
        } 

        // adding terminator to strings in buffer
        buffer[number_of_bytes_received] = '\0';

        // to ensure, that reveived string is correctly terminated
        // buffer[number_of_bytes_received] = '\0'; 
        

        
        // reply to client
        char *reply_msg = "Reply\r\n\r\n"; // reply message
        int length, bytes_sent; // length of the reply message and the bytes that a sent
        length = strlen(reply_msg); // get lentgh of the reply message
        char *last_part_of_received_msg = strstr(buffer, "\r\n\r\n"); // checking for specific sequence in the received message
        if (last_part_of_received_msg != NULL) { // if the \r\n\r\n sequence is found
            if (debug == 1) {
                printf("----------DEBUG------------- main() called\n");
                printf("last_part_of_received_msg: %s\n", last_part_of_received_msg);
                printf("----------DEBUG END-------------main() called\n");
            }
            bytes_sent = send(new_fd, reply_msg, length, 0); // send the reply message wiht send()
            if (bytes_sent < 0) { // if the message was not sent
                perror("ERROR while sending data");
                return 2; // specific error
            } else { // print the sent message
                printf("Sent: %s\n", reply_msg);
        }
        } else { // if the specific sequence was not found
            if(debug == 1) {
                printf("----------DEBUG------------- main() called\n");
                printf("last_part_of_received_msg: %s\n", last_part_of_received_msg);
                printf("----------DEBUG END-------------main() called\n");
            }
            perror("ERROR when receiving data. Apparently no packet was received");
        }
        
        // close connection to the client
        close(new_fd);
    }
    // close the socket 
    close(sockfd);  

    return 0;
}
    