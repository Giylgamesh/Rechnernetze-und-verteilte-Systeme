#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <ip_address> <port>\n", argv[0]);
        exit(1);
    }

    // Get the IP address and port from the command-line arguments
    char *ip_address = argv[1];
    int portno = atoi(argv[2]);

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Set the SO_REUSEADDR option to avoid port reuse issues
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(1);
    }

    // Set up the server address structure
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip_address);
    serv_addr.sin_port = htons(portno);

    // Bind the socket to the specified IP address and port
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error on binding");
        exit(1);
    }

    // Start listening for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("Error on listening");
        exit(1);
    }

    printf("Server listening on %s:%d\n", ip_address, portno);

    // Keep the server running indefinitely (you'll add the connection handling logic later)
    while (1) {
        // This is where you'll accept connections and handle requests in the next tasks
    }

    close(sockfd);
    return 0;
}