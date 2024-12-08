#include <sys/types.h> // datentypen definieren (u.a. size_t)
#include <sys/socket.h> // sockets erstellen und verwalten
#include <signal.h> //impliziert auf mac aber nicht bei allen anderen systemen
#include <netdb.h> // für das struct addrinfo (ip-adressen und ports)
#include <stdio.h> // standard ein- und ausgabe
#include <stdlib.h> // speicherverwaltung (u.a. malloc)
#include <string.h> // strings bearbeiten
#include <unistd.h> // posix-aufrufe (read, write, close)
#include <arpa/inet.h> // ip-adressen verwalten und konvertieren
#include <regex.h> // reguläre ausdrücke für strings
// max anfragegrösse ist 8kB + 40 header mit 256 bytes content_size = 18kB
#define MAX_REQUEST_SIZE 1024*18
#define MAX_RESOURCES 100
static char *const END_SEQUENCE = "\r\n\r\n";
regex_t rex_request;

// struct für http anfrage (geparsed)
struct http_request {
    int complete; // 0 == invalid
    char http_version[9]; // "HTTP/x.y+\0'
    char method[7]; // 'GET|PUT|POST|HEAD|PATCH|DELETE+\0'
    char uri[256];
    int content_size;
    char *body;
};

// Resource structure for dynamic content
struct resource {
    char *uri;                  // Pfad
    char *content;              // Inhalt
    struct resource *next;      // Pointer zum Nachfolger
};

// Head of the linked list
struct resource *resource_list_root = NULL;

void init_resource_list() {
    resource_list_root = malloc(sizeof(struct resource));
    resource_list_root->uri = NULL;
    resource_list_root->content = NULL;
    resource_list_root->next = NULL;
}

void free_resource_list() {
    struct resource *current_res = resource_list_root;
    while (current_res->next) {
        struct resource *next_res = current_res->next;
        free(current_res->uri);
        free(current_res->content);
        free(current_res);
        current_res = next_res;
    }
    resource_list_root = NULL;
}

struct resource *get_ressource(char *uri) {
    struct resource *current_res = resource_list_root->next;
    if (current_res != NULL) {
        while (current_res) {
            if (strcmp(current_res->uri, uri) == 0) {
                printf("[DEBUG] found ressource for uri: %s\n", uri);
                return current_res;
            } else {
                current_res = current_res->next;
            }
        }
    }
    printf("[DEBUG] no resource found for uri: %s\n", uri);
    return NULL;
}

int delete_resource(char *uri) {
    struct resource *current_res = resource_list_root->next;
    struct resource *previous_res = resource_list_root;
    if (current_res != NULL) {
        while (current_res) {
            if (strcmp(current_res->uri, uri) == 0) {
                previous_res->next = current_res->next;
                free(current_res->uri);
                free(current_res->content);
                free(current_res);
                printf("[DEBUG] successfully deleted uri: %s\n", uri);
                return 1;
            } else {
                previous_res = current_res;
                current_res = current_res->next;
            }
        }
    }
    printf("[DEBUG] Delete failed for missing uri: %s\n", uri);
    return 0;
}

// returns 1 on overwrite and 0 on create
int put_resource(char *uri, char *content) {
    int deleted = delete_resource(uri);

    struct resource *new_res = malloc(sizeof(struct resource));
    new_res->uri = malloc(strlen(uri) + 1);
    strcpy(new_res->uri, uri);
    new_res->content = malloc(strlen(content) + 1);
    strcpy(new_res->content, content);

    // link
    struct resource *current_res = resource_list_root;
    while (current_res->next) {
        current_res = current_res->next;
    }
    current_res->next = new_res;
    printf("[DEBUG] successfully %s uri: %s\n", deleted ? "updated" : "created", uri);
    return deleted;
}


// parse the current buffer into a http request
struct http_request *parse_request(char *buffer) {
    struct http_request *req = malloc(sizeof(struct http_request));
    memset(req, 0, sizeof(&req));

    regmatch_t matches[4];
    int res = regexec(&rex_request, buffer, 4, matches, 0);
    if (res == 0) {
        printf("[DEBUG] Complete HTTP request:\n%s\n", buffer);
        req->complete = 1;
        strncpy(req->method, buffer + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
        printf("[DEBUG] method: %s\n", req->method);
        // TODO Tino added
        // Wichtig NULL-Terminator!
        size_t method_len = matches[1].rm_eo - matches[1].rm_so;
        req->method[method_len] = '\0';`

        strncpy(req->uri, buffer + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
        printf("[DEBUG] uri: %s\n", req->uri);
        strncpy(req->http_version, buffer + matches[3].rm_so, matches[3].rm_eo - matches[3].rm_so);
        printf("[DEBUG] http version: %s\n", req->http_version);

        // extract Content-Length
        char *content_length_header = strstr(buffer, "Content-Length:");
        if (content_length_header) {
            sscanf(content_length_header, "Content-Length: %d", &req->content_size);
            printf("[DEBUG] Content-Length: %d\n", req->content_size);
        } else { printf("[DEBUG] No content length set\n"); }
    } else {
        char msgbuf[100] = {0};
        regerror(res, &rex_request, msgbuf, sizeof(msgbuf));
        //fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        req->complete = 0;
    }
    return req;
}

static struct sockaddr_in derive_sockaddr(const char *host, const char *port) {
    struct addrinfo hints = {.ai_family = AF_INET,};
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

// precompile regex for request line parsing
void init_regex() {
    int res = regcomp(&rex_request, "^(GET|DELETE|PUT|GETD|HEAD|PATCH) (/[^ ]*) (HTTP/(1\\.0|1\\.1|2|3))",
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

    char *response;
    char *dynamic_response_buffer = NULL;

    if (req->complete) {
        printf("[DEBUG] Handling %s %s\n", req->method, req->uri);

        // HANDLE GET REQUEST
        if (strcmp("GET", req->method) == 0) {
            printf("[DEBUG] Processing GET request\n");
            if (strcmp(req->uri, "/static/foo") == 0) {
                response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nFoo\r\n";
            } else if (strcmp(req->uri, "/static/bar") == 0) {
                response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nBar\r\n";
            } else if (strcmp(req->uri, "/static/baz") == 0) {
                response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nBaz\r\n";
            } else if (strncmp(req->uri, "/dynamic/", 9) == 0) {
                // handle request for dynamically created content
                struct resource *r = get_ressource(req->uri);
                if (r != NULL) {
                    printf("[DEBUG] Return content for URI\n");
                    dynamic_response_buffer = malloc(MAX_REQUEST_SIZE);
                    memset(dynamic_response_buffer, 0, MAX_REQUEST_SIZE);
                    int content_length = strlen(r->content);

                    sprintf(dynamic_response_buffer, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s\r\n",
                            content_length, r->content);
                    response = dynamic_response_buffer;

                    printf("!!!! BUFFER '%s'\n", dynamic_response_buffer);

                } else {
                    printf("[DEBUG] GET request URI not found\n");
                    response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                }

            } else {
                printf("[DEBUG] GET request URI not found\n");
                response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            }
        }
            // HANDLE PUT REQUEST
        else if (strcmp("PUT", req->method) == 0) {
            printf("[DEBUG] Processing PUT request\n");
            if (strncmp(req->uri, "/dynamic/", 9) == 0) {
                int updated = put_resource(req->uri, req->body);
                if (updated == 1) {
                    response = "HTTP/1.1 204 No Content\r\n\r\n";
                } else {
                    response = "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n";
                }
            } else {
                printf("[DEBUG] PUT request forbidden URI\n");
                response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
            }
        }
            // HANDLE DELETE REQUEST
        else if (strcmp("DELETE", req->method) == 0) {
            printf("[DEBUG] Processing DELETE request\n");
            // prüfe ob pfad dynamisch ist
            if (strncmp(req->uri, "/dynamic/", 9) == 0) {

                int deleted = delete_resource(req->uri);
                if (deleted == 1) {
                    printf("[DEBUG] Deleted resource: %s\n", req->uri);
                    response = "HTTP/1.1 204 No content\r\n\r\n";
                } else {
                    printf("[DEBUG] Resource to delete not found: %s\n", req->uri);
                    response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                }
            } else {
                printf("[DEBUG] DELETE request forbidden URI\n");
                response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
            }
        }
            // Nicht unterstützte Anfragen, Fehlermeldung
        else {
            printf("[DEBUG] Method not implemented: %s\n", req->method);
            response = "HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\n\r\n";
        }

    } else {
        // unvollständige anfrage
        printf("[DEBUG] Incomplete request\n");
        response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
    }

    // Antwort an den client senden
    ssize_t bytes_sent = write(client_socket, response, strlen(response));
    if (bytes_sent == -1) {
        perror("Error sending response");
    } else {
        printf("[DEBUG] Response sent: '%s'\n", response);
    }
    free(dynamic_response_buffer);
}

// main funktion, bekommt argumente (port und adresse)
int main(int argc, char *argv[]) {
    // initialization
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("Unable to install signal handler!");
        exit(EXIT_FAILURE);
    }

    init_regex();
    init_resource_list();

    // anzahl der erhaltenen argumente prüfen (adresse, port)
    if (argc != 3) {
        printf("Anzahl der Argumente inkorrekt!\n");
        exit(EXIT_FAILURE);
    }
    // addresse sichern
    const char *address = argv[1];
    const char *cport = argv[2];

    // PORT UMWANDELN UND PRÜFEN
    // erhaltenen port in integer umwandeln
    int iport = atoi(argv[2]);

    // prüfen, ob das umwandeln geklappt hat
    if (iport <= 0 || iport > 65535) {
        printf("Port ist ungültig!\n");
        exit(EXIT_FAILURE);
    }
    // IP ADRESSE PARSEN
    // ip adresse parsen
    struct sockaddr_in server_address = derive_sockaddr(address, cport);

    // SOCKETS ERSTELLEN
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

    // SERVER AUFSETZEN
    // server adresse aufsetzen
    // adressfamilie
    server_addr.sin_family = AF_INET;

    // server an alle verfügbaren netzwerkschnittstellen binden
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // port in das richtige byte-format für netzwerkkommunikation übersetzen
    server_addr.sin_port = htons(iport);

    // funktion um socket zu binden, also eine ip adresse und einen port zuzuweisen
    int bind_result = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    // prüfe ob alles geklappt hat
    if (bind_result == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // lauschen und das ergebnis in der variable sichern FIXME set proper backlog content_size, formerly 10
    int listen_result = listen(server_socket, 1);

    // prüfe ob alles geklappt hat
    if (listen_result == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    // info ausgeben, der server lauscht gerade
    printf("[DEBUG] Server listening on port %d...\n", iport);

    // temporärer speicher für Anfragen
    char request_buffer[MAX_REQUEST_SIZE] = {0};

    // endlosschleife wartet auf eine verbindung
    printf("[DEBUG] Endlosschleife wartet auf Verbindung\n");
    while (running) {
        // verbindung akzeptieren
        int client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &address_length);

        // prüfe ob alles geklappt hat, sonst weiter warten...
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }
        printf("[DEBUG] Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        while(1) {
            // lese Anfrage in den Puffer, maximale Anfragegröße MAX_REQUEST_SIZE
            memset(request_buffer, 0, MAX_REQUEST_SIZE);
            // lese zeichenweise, bis die Sequenz '\r\n\r\n' gefunden wird
            int buffer_position = 0;
            const int SEQ_LEN = strlen(END_SEQUENCE);
            ssize_t bytes_received = 0;

            struct http_request *req = NULL;
            int header_end = 0;
            int body_parsed = 0;

            do {
                bytes_received = read(client_socket, request_buffer + buffer_position,
                                      MAX_REQUEST_SIZE - buffer_position - 1);
                printf("read bytes: %ld", bytes_received);
                if (bytes_received == 0) {
                    printf("[DEBUG] Finished reading. No more data from client\n");
                    goto out;
                } else if (bytes_received < 0) {
                    perror("could not read from client socket!\n");
                    goto out;
                }
                // erfolgreich gelesen...
                buffer_position += bytes_received;
                // parse http request header soweit verfügbar
                if (body_parsed == 0 && buffer_position >= SEQ_LEN) {
                    char *he = strstr(request_buffer, END_SEQUENCE);
                    header_end = he - (char *) request_buffer;

                    printf("[DEBUG] Received:\n%s\n", request_buffer);
                    printf("[DEBUG] end of request detected, start parsing...");

                    printf("[DEBUG] Request buffer: %s", request_buffer);
                    // anfragebuffer als http request parsen
                    req = parse_request(request_buffer);
                    body_parsed = 1;

                    // kein request body
                    if (req->content_size < 1) {
                        break;
                    }
                }
                // Lese verbliebenen body raus
                if (body_parsed == 1 && (buffer_position - header_end) >= req->content_size) {
                    // allocate buffer
                    req->body = malloc(req->content_size + 1);
                    memset(req->body, 0, req->content_size + 1);
                    memcpy(req->body, request_buffer + header_end + SEQ_LEN, req->content_size);
                    break;
                }

            } while (buffer_position < MAX_REQUEST_SIZE);


            // GET/PUT/DELETE logik
            handle_http_request(client_socket, req);

            // free the request
            if (req) {
                if (req->body) { free(req->body); }
                free(req);
            }
        }
        out:
        // client socket schliessen
        close(client_socket);
        printf("[DEBUG] Closing client socket");
    }

    // free compiled regex
    regfree(&rex_request);

    // ressourcen aufraeumen
    free_resource_list();

    // server socket schließen, eventueller Fehler als Exit-Code setzen
    return close(server_socket);
}
