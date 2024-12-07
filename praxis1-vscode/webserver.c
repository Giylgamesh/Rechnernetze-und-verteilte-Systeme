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


// debug flag
int debug = 0;

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

    struct addrinfo hints, *res, *P;
    int status; 

    // check for command line arguments to get the host and port
    const char *host = argv[1];
    const char *port = argv[2];
    if (argc < 3) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    } else {
        host = argv[1]; // get host from command-line argument
        port = argv[2]; // get port from command-line argument
    }
    printf("HOST: %s -----> PORT TO USE: %s", host, port); 

    // // check for command line arguments 
    // // fprintf() sends output to specific stream, 
    // // in this case stderr (standard error, its separate from stdout (standard output) which is the output of the program)
    // if (argc != 2) {
    //     fprintf(stderr,"usage: showip hostname\n");
    //     return 1;
    // }


    memset(&hints, 0, sizeof hints); // zeroing out the memory so that the struct is empty
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC can be either AF_INET or AF_INET6 (IPv4 or IPv6)
    hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM is TCP , SOCK_DGRAM is UDP
    hints.ai_flags = AI_PASSIVE; // assign the address of my local host to the socket structures

    status = getaddrinfo(NULL, port, &hints, &res);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
    // again, you should do error-checking on getaddrinfo(), and walk
    // the "res" linked list looking for valid entries instead of just
    // assuming the first one is good (like many of these examples do).
    // See the section on client/server for real examples.

    // create a socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd <  0) {
        perror("ERROR while creating a socket"); // fprintf(stderr, "socket: %s\n", strerror(errno));
        return 2; // specific error
    }


    // bind it to the port we passed in to getaddrinfo():
    // bind() returns 0 of successfull, -1 for errors
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("ERROR while binding a socket"); // fprintf(stderr, "bind: %s\n", strerror(errno));
        return 2; // specific error
    }

    // listen for incoming connections on the socket sockfd
    int backlog = 3;  // maximum number of connections waiting in queue
    listen(sockfd, backlog);
   
    // accept a connection attempt to a speciic socket
    /*addr will usually be a pointer to a local struct sockaddr_storage. 
    This is where the information about the incoming connection will go 
    (and with it you can determine which host is calling you from which port)*/
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    addr_size = sizeof(their_addr);
    new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size); // only relevant for TCP protocol, becouse we build a continues data stream
    if (new_fd < 0) {
        perror("ERROR while accepting a connection");
        return 2; // specific error
    }
    
    
    // receive data from the socket
    char buffer[1024]; // buffer to store the incoming data
    int number_of_bytes_received = recv(new_fd, buffer, sizeof(buffer), 0); // recv() returns the number of bytes received, or -1 if an error occurred or 0 if the connection is closed
    if (number_of_bytes_received < 0) {
        perror("ERROR while receiving data");
        return 2; // specific error
    } if (number_of_bytes_received == 0) {
        printf("connection closed on other end!\n");
    } else {
        buffer[number_of_bytes_received] = '\0';
        printf("Received: %s\n", buffer);
    }
    
    // reply to client
    char *reply_msg = "Reply"; // reply message
    int length, bytes_sent; // length of the reply message and the bytes that a sent
    length = strlen(reply_msg); // get lentgh of the reply message
    bytes_sent = send(new_fd, reply_msg, length, 0); // send the reply message wiht send()
    // check if the message was sent successfully
    if (bytes_sent < 0) { // if the message was not sent
        perror("ERROR while sending data");
        return 2; // specific error
    } else { // print the sent message
        printf("Sent: %s\n", reply_msg);
    }


    // // getaddrinfo() returns a linked list of addrinfo structures
    // status = getaddrinfo(argv[1], NULL, &hints, &res);
    // if (status  != 0) {
    //     fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    //     return 2;
    // }

    freeaddrinfo(res); // free the linked list

    // close the socket
    close(new_fd);
    close(sockfd);

    return 0;
}