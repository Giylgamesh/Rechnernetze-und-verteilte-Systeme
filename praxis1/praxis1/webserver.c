#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BACKLOG 10 // Maximale Anzahl an Connections in der Listen-Queue
#define MAXDATASIZE 8192 // Maximale Request und Response Größe in Bytes

/*
* Gibt die IPv4 oder IPv6-Adresse des mitgebenen sockaddr structs zurück
**/
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

char *ok_req = "HTTP/1.1 200 OK\r\n\r\n";
char *bad_req = "HTTP/1.1 400 Bad Request\r\n\r\n";

/*
* Gibt die korrekte Response-Message mit Status-Code zurück, je nach der HTTP-Semantik
**/
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

    /*
    * Holt IP-Adresse und Port für den Server aus den Argumenten beim ausführen des Servers.
    **/
    const char *host = argv[1];
    const char *port = argv[2];

    printf("\n\n----------------------------------- Starting Webserver -----------------------------------\n\n");
    printf("Host: %s Port: %s\n\n", host, port);

    memset(&hints, 0, sizeof hints); // struct sollte leer sein
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6 (aus dem GUIDE)
    hints.ai_socktype = SOCK_STREAM; // TCP Stream Socket

    /*
    * Speichert die mitgegebenen Daten in eine Referenz auf einen Struct
    **/
    if ((status = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }


    /*
    * Initialisiert den Socket anhand den gegebenen Serverinformationen
    **/
    if ((sock_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
        fprintf(stderr, "socket() init error: %s\n", gai_strerror(status));
        exit(1);
    }

    /*
    * Bindet den Socketdescriptor an den Socket
    **/
    if ((status = bind(sock_fd, servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
        fprintf(stderr, "bind error: %s\n", gai_strerror(status));
    }

    /*
    * Setzt Optionen für den Server-Socket, zb. dass die Adresse bzw. der PORT erneut verwendet werden kann.
    **/
    int yes=1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt failed...");
        exit(1);
    }

    /*
    * Wartet/Horcht auf eingehende Connections zum Server.
    **/
    printf("----------------------------------- Opening Listen Socket -----------------------------------\n\n");
    listen(sock_fd, BACKLOG);

    /*
    * Die erste Verbindungsanfrage aus der Listen-Queue, wird versucht anfzubauen.
    **/
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
        /*
        * Der Datenstrom bis zum Byte [MAXDATASIZE-1] wird ausgelesen und in den Buffer gespeichert
        **/
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

            /*
             * Generiert eine Response-Antwort mit Statuscode und sendet diese an den Client zurück.
             */
            char msg[MAXDATASIZE];
            get_response(msg);
            int len, bytes_sent;
            len = strlen(msg);
            bytes_sent = send(client_fd, msg, len, 0);

            /*
             * move_index errechnet sich aus dem buf_index also der länge des Buffers minus
             * der verbleibenden Länge des Buffers nachdem ein Paket erkannt wurde, also
             * nach \r\n\r\n. (Wichtig, dass sind insgesamt nur 4 Bytes <-- "\r\n\r\n").
             *
             * Da recv_buf an den Anfang des Buffer zeigt und somit in der Adresse "kleiner" ist und
             * recv_end an das Ende des Buffers zeigt und somit eine "größere" Adresse hat,
             * subtrahieren wir einfach beide Zeiger (+4 Bytes wegen dem Paketende).
             * Diese Differenz ist die Anzahl an Bytes (bzw. einzelnen Adresschritten/Indizes durch das Array),
             * zwischen recv_end + 4 Bytes weiter und recv_buf
             */
            int move_index = buf_index - (recv_end + 4 - recv_buf);
            memmove(recv_buf, recv_buf + (recv_end + 4 - recv_buf), move_index);
            buf_index = move_index;
            printf("Sending Message with %d Bytes\n\n", bytes_sent);
        }
    }

    /*
    * Schließt alle verbundenen FDs bzw. den Socket und berreinigt den Speicher.
    **/
    close(client_fd);
    close(sock_fd);
    freeaddrinfo(servinfo);
    printf("----------------------------------- Server fully Closed -----------------------------------\n\n");
    return 0;
}