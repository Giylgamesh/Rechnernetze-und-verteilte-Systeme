#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

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