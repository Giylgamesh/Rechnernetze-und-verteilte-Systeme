// #include "server_sructs.h" 
// #include "helper_functions.h"
// #include <stdio.h> // for printf
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h> // for AI_PASSIVE
// #include <stdlib.h> // for exit()


// // simple call for server to listen on hosts IP address , port 3490
// // does not actually listen, just sets up the socket
// int main() {
//     int status; // return value for getaddrinfo
//     struct addrinfo hints; 
//     struct addrinfo *servinfo;  // will point to the results

    // // memset() fills a block of memory with a particular value
    // // make sure the struct is empty
    // memset(&hints, 0, sizeof hints); 
    // // AF = Address Family
    // hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    // hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    // // hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

//     status = getaddrinfo("www.example.com", "3490", &hints, &servinfo);

//     if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
//         fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
//         exit(1);
//     }

//     // servinfo now points to a linked list of 1 or more struct addrinfos

//     // ... do everything until you don't need servinfo anymore ....

//     freeaddrinfo(servinfo); // free the linked-list

//     return 0;
// }




/*
** showip.c -- show IP addresses for a host given on the command line
*/

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

// debug flag
int debug = 1;

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
    
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];   // array with char elements of size INET6_ADDRSTRLEN 
                                    //(INET6_ADDRSTRLEN is the maximum length of the IPv6 address string)

    
    // check for command line arguments 
    // fprintf() sends output to specific stream, 
    // in this case stderr (standard error, its separate from stdout (standard output) which is the output of the program)
    if (argc != 2) {
        fprintf(stderr,"usage: showip hostname\n");
        return 1;
    }


    memset(&hints, 0, sizeof hints); // zeroing out the memory so that the struct is empty
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC can be either AF_INET or AF_INET6 (IPv4 or IPv6)
    hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM is TCP , SOCK_DGRAM is UDP

    // getaddrinfo() returns a linked list of addrinfo structures
    status = getaddrinfo(argv[1], NULL, &hints, &res);
    if (status  != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n\n", argv[1]);

    for(p = res;p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res); // free the linked list

    return 0;
}