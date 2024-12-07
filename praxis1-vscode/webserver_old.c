#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


// we define size of buffer to 1024 bytes 
// this will be used to store the incoming data
#define BUFFER_SIZE 1024
// we define the port number to 8080
// this will be used to listen for incoming connections on this port
// not sure if we need it for the task
#define PORT 8080

int main() {

    // struct addrinfo address; // initiate struct for the server address .
    struct sockaddr_in address; // initiate struct for the server address . 
    //This struct will contain the address of the server and the port number
    int addrlen = sizeof(address); //define size of address

    int new_socket; // initiate variable for the accepted client connection
    int server_fd;     // initiate file descriptor (fd)  for the server socket.
    
    int opt = 1; // variable for the setsockopt function to set the socket option

    char buffer[BUFFER_SIZE] = {0}; // buffer to store the incoming data of data type char
    
    // define reply message as 
    char *reply = "Reply"; 


    // if the socket function fails, print an error message and exit the program with a failure status
    // AF_INET is the address family for IPv4
    // AF_INET6 for IPv6
    // AF_UNSPEC for unspecified.... either IPv4 or IPv6
    // SOCK_STREAM is the type of socket for TCP
    // 0 is the protocol value for IP and 1 is the protocol value for ICMP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed"); // perror() prints error message to stderr instead of stdout (standard output) like printf()
        exit(EXIT_FAILURE); // close program and server
    }
    
    // Forcefully attaching socket to the port 8080
    // setting socket options to SO_REUSEADDR like described in task, to avoid the "Address already in use" error
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
    
    // val
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    if  (valread < 0) {
        perror("read");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Received: %s\n", buffer);
    
    // Close the socket
    close(new_socket);
    close(server_fd);
    
    return 0;
}