#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BACKLOG 10     // Maximale Anzahl an Connections in der Listen-Queue
#define MAXDATASIZE 100

int main(int argc, char *argv[])
{
    char buf[MAXDATASIZE];
    struct sockaddr_storage their_addr;
    int status;
    int sockfd, new_fd;
    socklen_t addr_size;
    struct addrinfo hints, *servinfo;

    const char *host = argv[1];
    const char *port = argv[2];

    printf("HOST: %s -----> PORT TO USE: %s", host, port);

    memset(&hints, 0, sizeof hints); // struct sollte leer sein
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6 (GUIDE)
    hints.ai_socktype = SOCK_STREAM; // TCP stream socket


    // Adresse, Port und Einstellungen initialisieren
    if ((status = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }


    // Socket initialisieren
    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    printf("\nSocket() File Descriptor value: %d\n\n", sockfd);


    // Socket-Discriptor an die Adresse Binden
    if ((status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
        fprintf(stderr, "bind error: %s\n", gai_strerror(status));
    }


    // Auf Connections Horchen/Listen
    printf("\nOpening Listen Socket...\n\n");
    listen(sockfd, BACKLOG);


    // Neue Verbindungen aus der Listen-Queue akzeptieren
    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_fd == -1) {
        perror("Couldn't accept any Connection!");
    }

    /*int numbytes;
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    printf("Client: '%s'\n",buf);
     */

    // Antwort bei Erfolgreicher Verbindung zum Socket
    char *msg = "Thank you for connection to this Socket!";
    int len, bytes_sent;
    len = strlen(msg);
    bytes_sent = send(new_fd, msg, len, 0);

    close(sockfd);
    close(new_fd);

    freeaddrinfo(servinfo);
    
    return 0;
}