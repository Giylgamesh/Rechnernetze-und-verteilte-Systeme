#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd;     // initiate file descriptor for the server socket.
    int new_socket; // initiate file descriptor for the accepted client connection.
    struct sockaddr_in address; // initiate struct for the server address .
    int opt = 1; // variable for the setsockopt function to set the socket option
    int addrlen = sizeof(address); //define size of address
    char buffer[BUFFER_SIZE] = {0}; // buffer to store the incoming data
    
    // if the socket function fails, print an error message and exit the program with a failure status
    // AF_INET is the address family for IPv4
    // SOCK_STREAM is the type of socket for TCP
    // 0 is the protocol value for IP and 1 is the protocol value for ICMP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed"); // print error message
        exit(EXIT_FAILURE); 
    }
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt"); // print error message if setsockopt fails
        close(server_fd); // command to close server socket
        exit(EXIT_FAILURE); 
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Listening on port %d...\n", PORT);
    
    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Read data from the socket
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    printf("Received: %s\n", buffer);
    
    // Close the socket
    close(new_socket);
    close(server_fd);
    
    return 0;
}