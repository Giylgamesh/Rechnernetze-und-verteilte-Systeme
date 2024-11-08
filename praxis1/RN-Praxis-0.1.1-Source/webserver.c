#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> // für das struct addrinfo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <regex.h>

// max request size is 8kB + 40 headers of 256 bytes size = 18kB
#define MAX_REQUEST_SIZE 1024*18

static char *const END_SEQUENCE = "\r\n\r\n";
regex_t rex_request;

// holds a parsed http request
struct http_request {
    int complete; // 0 == invalid
    char http_version[9]; // "HTTP/x.y+\0'
    char method[7]; // 'GET|PUT|POST|HEAD|PATCH|DELETE+\0'
    char uri[256];
    int size;
    char *body;
};

// parse the current buffer into a http request
struct http_request parse_request(char *buffer) {
    struct http_request req = {
            .complete = 0,
            .size = 0,
            .body = NULL,
            .http_version = {0},
            .uri = {0},
            .method = {0},
    };

    regmatch_t matches[4];
    int res = regexec(&rex_request, buffer, 4, matches, 0);
    if (res == 0) {
        printf("complete http request:\n%s\n", buffer);
        req.complete = 1;
        strncpy(req.method, buffer + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
        printf("method: %s\n", req.method);
        strncpy(req.uri, buffer + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
        printf("uri: %s\n", req.uri);
        strncpy(req.http_version, buffer + matches[3].rm_so, matches[3].rm_eo - matches[3].rm_so);
        printf("http version: %s\n", req.http_version);
    } else {
        // TODO ausgabe entfernen
        char msgbuf[100] = {0};
        regerror(res, &rex_request, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        req.complete = 0;
    }
    return req;
}

static struct sockaddr_in derive_sockaddr(const char *host, const char *port) {
    struct addrinfo hints = {
            .ai_family = AF_INET,
    };
    struct addrinfo *result_info;

    // Resolve the host (IP address or hostname) into a list of possible addresses.
    int returncode = getaddrinfo(host, port, &hints, &result_info);
    if (returncode) {
        fprintf(stderr, "Error parsing host/port\n");
        exit(EXIT_FAILURE);
    }

    // Copy the sockaddr_in structure from the first address in the list
    struct sockaddr_in result = *((struct sockaddr_in *) result_info->ai_addr);

    // Free the allocated memory for the result_info
    freeaddrinfo(result_info);
    return result;
}

// -------------------------------------------------------------------------

void init_regex() {// precompile regex for request line parsing
    int res = regcomp(&rex_request, "^(GET|DELETE|PUT|POST|HEAD|PATCH) (/[^ ]*) (HTTP/(1\\.0|1\\.1|2|3))",
                      REG_EXTENDED);
    if (res) {
        fprintf(stderr, "error compiling regex\n");
        exit(3);
    }
}

// server socket
int running = 1;
int server_socket;

void signal_handler(int signal_number) {
    if (signal_number == SIGINT || signal_number == SIGTERM) {
        printf("Received signal %d. Server is going down now...\n", signal_number);
        running = 0;
        close(server_socket);
    } else {
        printf("Received signal %d.\n", signal_number);
    }
}

void handle_http_request(int client_socket, struct http_request *req) {
    // valid request line
    if (req->complete) {
        printf("handle %s %s\n", req->method, req->uri);

        // 400: inkorrekte Anfragen
        // 404: GET-Anfragen
        // 501: alle anderen Anfragen
        char *response;
        if (strcmp("GET", req->method) == 0) {

            // fake file resources static/foo, static/bar, static/baz
            if (strcmp(req->uri, "/static/foo") == 0) {
                response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nFoo\r\n";
            } else if (strcmp(req->uri, "/static/bar") == 0) {
                response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nBar\r\n";
            } else if (strcmp(req->uri, "/static/baz") == 0) {
                response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nBaz\r\n";
            } else {
                response = "HTTP/1.1 404 Not Found\r\n\r\n";
            }

        } else {
            response = "HTTP/1.1 501 Not Implemented\r\n\r\n";
        }

        // send the response to the client
        ssize_t bytes_sent = write(client_socket, response, strlen(response));

        // prüfen ob die antwort versendet wurde
        if (bytes_sent == -1) {
            perror("Error sending response");
        } else {
            // info ausgeben welche antwort versendet wurde
            printf("Response sent:\n%s\n", response);
        }
    } else {
        // send bad request
        printf("Send bad request for incomplete request");
        const char *response = "HTTP/1.1 400 Bad Request\r\n\r\n";
        ssize_t bytes_sent = write(client_socket, response, strlen(response));
        if (bytes_sent == -1) {
            perror("Error sending error response");
        }
    }
}

// main funktion, bekommt argumente (port und adresse)
int main(int argc, char *argv[]) {
    // initialization
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("Unable to install signal handler!");
        exit(EXIT_FAILURE);
    }

    init_regex();

    // anzahl der erhaltenen argumente prüfen (adresse, port)
    if (argc != 3) {
        printf("Anzahl der Argumente inkorrekt!\n");
        exit(EXIT_FAILURE);
    }
    // addresse sichern
    const char *address = argv[1];
    const char *cport = argv[2];

    // erhaltenen port in integer umwandeln
    int iport = atoi(argv[2]);

    // prüfen, ob das umwandeln geklappt hat
    if (iport <= 0 || iport > 65535) {
        printf("Port ist ungültig!\n");
        exit(EXIT_FAILURE);
    }

    // ip adresse parsen
    struct sockaddr_in server_address = derive_sockaddr(address, cport);

    // struct um informationen zu speichern
    // sockaddr_in is eine spezielle struktur, um IP adressen darzustellen
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    // socklen_t ist ein spezieller datentyp für die länge einer socket adresse
    socklen_t address_length = sizeof(client_addr);

    // socket erstellen
    // AF_INET ist die adressfamilie
    // SOCK_STREAM steht für streaming socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // enforce immediate closing of the IP-port (to avoid bind errors on server restart)
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int));
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &(int) {1}, sizeof(int));

    // prüfen ob alles geklappt hat
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // server adresse aufsetzen
    // adressfamilie
    server_addr.sin_family = AF_INET;

    // server an alle verfügbaren netzwerkschnittstellen binden
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // port in das richtige byte-format für netzwerkkommunikation übersetzen
    server_addr.sin_port = htons(iport);

    // funktion um socket zu binden, also eine ip adresse und einen port zuzuweisen
    int bind_result = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    // prüfen ob alles geklappt hat
    if (bind_result == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // lauschen und das ergebnis in der variable sichern FIXME set proper backlog size, formerly 10
    int listen_result = listen(server_socket, 10);

    // prüfe ob alles geklappt hat
    if (listen_result == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    // info ausgeben, der server lauscht gerade
    printf("Server listening on port %d...\n", iport);

    // temporärer speicher für Anfragen
    char request_buffer[MAX_REQUEST_SIZE] = {0};

    // endlosschleife wartet auf eine verbindung
    while (running) {
        // verbindung akzeptieren
        int client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &address_length);

        // prüfe ob alles geklappt hat, sonst weiter warten...
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        // infos zum client socket ausgeben
        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // lese Anfrage in den Puffer, laut Aufgabenstellung ist die maximale Anfragegröße
        while (1) {
            memset(request_buffer, 0, MAX_REQUEST_SIZE);
            // lese zeichenweise, bis die Sequenz '\r\n\r\n' gefunden wird
            int end_sequence = 0;
            int buffer_position = 0;
            const int SEQ_LEN = strlen(END_SEQUENCE);
            while (!end_sequence && buffer_position < MAX_REQUEST_SIZE) {
                ssize_t bytes_received = read(client_socket, request_buffer + buffer_position, 1);
                if (bytes_received == 0) {
                    printf("No more data from client\n");
                    goto out;
                } else if (bytes_received < 0) {
                    perror("could not read from client socket!\n");
                    goto out;
                }
                // erfolgreich gelesen...
                buffer_position += bytes_received;
                if (buffer_position >= 3) {
                    if (strcmp(END_SEQUENCE, request_buffer + buffer_position - SEQ_LEN) == 0) {
                        printf("end of request detected, start parsing...");
                        end_sequence = 1;
                    }
                }
            }

            printf("Received:\n%s\n", request_buffer);

            // try parsing current request_buffer as http_request
            struct http_request req = parse_request(request_buffer);

            handle_http_request(client_socket, &req);
        }
        out:
        // close client socket
        close(client_socket);
    }

    // free compiled regex
    regfree(&rex_request);

    // server socket schließen
    return close(server_socket);
}