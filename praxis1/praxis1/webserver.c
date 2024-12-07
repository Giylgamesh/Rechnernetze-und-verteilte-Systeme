#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BACKLOG 10     // Maximale Anzahl an Connections in der Listen-Queue
#define MAXDATASIZE 8192

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

char *ok_req = "HTTP/1.1 200 OK\r\n\r\n";
char *bad_req = "HTTP/1.1 400 Bad Request\r\n\r\n";

void get_response(char *msg) {
    memcpy(msg, bad_req, strlen(bad_req));
    msg[strlen(bad_req)] = '\0';
}

int main(int argc, char *argv[])
{
    char buf[MAXDATASIZE];
    struct sockaddr_storage client_addr;
    int status;
    int sock_fd, client_fd;
    socklen_t addr_size;
    struct addrinfo hints, *servinfo;
    char s[INET6_ADDRSTRLEN];

    const char *host = argv[1];
    const char *port = argv[2];

    printf("\n\n----------------------------------- Starting Webserver -----------------------------------\n\n");
    printf("Host: %s Port: %s\n\n", host, port);

    memset(&hints, 0, sizeof hints); // struct sollte leer sein
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6 (GUIDE)
    hints.ai_socktype = SOCK_STREAM; // TCP stream socket


    // Adresse, Port und Einstellungen initialisieren
    if ((status = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }


    // Socket initialisieren
    if ((sock_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
        fprintf(stderr, "socket() init error: %s\n", gai_strerror(status));
        exit(1);
    }

    // Socket-Discriptor an die Adresse Binden
    if ((status = bind(sock_fd, servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
        fprintf(stderr, "bind error: %s\n", gai_strerror(status));
    }

    int yes=1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt failed...");
        exit(1);
    }

    // Auf Connections Horchen/Listen
    printf("----------------------------------- Opening Listen Socket -----------------------------------\n\n");
    listen(sock_fd, BACKLOG);

    // Neue Verbindungen aus der Listen-Queue akzeptieren
    addr_size = sizeof client_addr;
    client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addr_size);
    printf("----------------------------------- First Connection -----------------------------------\n\n");
    printf("\nTrying to accept Connection...\n\n");
    if (client_fd == -1) {
        perror("Couldn't accept Connection!");
    }

    inet_ntop(client_addr.ss_family,
              get_in_addr((struct sockaddr *)&client_addr),
              s, sizeof s);
    printf("Server accepted connection from %s\n\n", s);

    int numbytes;
    int buf_index = 0;
    char recv_buf[MAXDATASIZE];
    memset(recv_buf, 0, MAXDATASIZE);
    while(1)  {
        printf("--------------------------- Waiting for Request from %s ---------------------------\n\n", s);
        if ((numbytes = recv(client_fd, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv failed...");
            exit(1);
        }

        if (numbytes == 0) {
            printf("No Bytes received, closing Server...\n\n");
            break;
        }

        buf[numbytes] = '\0';
        printf("Client Request: '%s'\n\n",buf);

        /*
         * Der main Buffer buf wird an den temporären Receive Buffer recv_buf angehängt
         * Die stelle an der die Angehängt wird entspricht index += numbytes durch jede Iteration,
         * wo das Paket noch nicht beendet wurde.
         *
         **/
        memcpy(recv_buf + buf_index, buf, numbytes);
        buf_index += numbytes;

        /*
         * Nullterminator muss, NACHDEM bufindex erhöht wird, gesetzt werden und nicht davor!
         * Hier wird außerdem noch der main Buffer buf "gecleared", also mit 0 intitialisiert.
         *
         **/
        recv_buf[buf_index] = '\0';
        memset(&buf, 0, numbytes);

        printf("Buffer recv_buf copy from buf: '%s'\n\n", recv_buf);

        // Überprüft den ganzen Buffer nach der Paket-Endung
        char *recv_end = strstr(recv_buf, "\r\n\r\n");

        /*
         * Bei erfolgreichem finden eines Paketendes wird eine Antwort versendet
         **/
        if (recv_end != NULL) {
            printf("Recieved Package... \n\n");
            // Antwort bei Erfolgreicher Verbindung zum Socket und korrekter Paket-Endung
            char msg[MAXDATASIZE];
            //char *msg = "Reply\r\n\r\n";
            get_response(msg);
            int len, bytes_sent;
            len = strlen(msg);
            bytes_sent = send(client_fd, msg, len, 0);

            /*
             * move_index errechnet sich aus dem buf_index also der länge des Buffers minus
             * der verbleibenden Länge des Buffers nachdem ein Paket erkannt wurde, also
             * nach \r\n\r\n.
             */
            int move_index = buf_index - (recv_end + 4 - recv_buf);
            memmove(recv_buf, recv_buf + (recv_end + 4 - recv_buf), move_index);
            buf_index = 0;
            printf("Sending Message with %d Bytes\n\n", bytes_sent);
        }
    }

    close(client_fd);
    close(sock_fd);
    freeaddrinfo(servinfo);
    printf("----------------------------------- Server fully Closed -----------------------------------\n\n");
    return 0;
}