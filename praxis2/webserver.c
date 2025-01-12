#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "data.h"
#include "http.h"
#include "util.h"

#define MAX_RESOURCES 100

struct tuple resources[MAX_RESOURCES] = {
    {"/static/foo", "Foo", sizeof "Foo" - 1},
    {"/static/bar", "Bar", sizeof "Bar" - 1},
    {"/static/baz", "Baz", sizeof "Baz" - 1}};

struct udp_msg {
    uint16_t msg_type;
    uint16_t hash_id;
    uint16_t node_id;
    struct in_addr node_ip;
    uint16_t node_port;
};

struct node_addr {
    uint16_t node_id;
    struct in_addr node_ip;
    uint16_t node_port;
};


int check_is_responsible(uint16_t curr_node_id, struct node_addr pred_node, uint16_t uri_hash, struct node_addr succ_node) {
    // is responsible
    if (pred_node.node_id < curr_node_id && uri_hash > pred_node.node_id && uri_hash <= curr_node_id) {
        return 0;
    }
    if (pred_node.node_id > curr_node_id && (uri_hash > pred_node.node_id || uri_hash <= curr_node_id)) {
        return 0;
    }
    // is not responsible but successor
    if (curr_node_id < succ_node.node_id && uri_hash > curr_node_id && uri_hash <= succ_node.node_id) {
        return 1;
    }
    if (curr_node_id > succ_node.node_id && (uri_hash > curr_node_id || uri_hash <= succ_node.node_id)) {
        return 1;
    }
    return -1;
}

/**
 * Sends an HTTP reply to the client based on the received request.
 *
 * @param conn      The file descriptor of the client connection socket.
 * @param request   A pointer to the struct containing the parsed request
 * information.
 */
void send_reply(int conn, struct request *request) {

    // Create a buffer to hold the HTTP reply
    char buffer[HTTP_MAX_SIZE];
    char *reply = buffer;
    size_t offset = 0;

    fprintf(stderr, "Handling %s request for %s (%lu byte payload)\n",
            request->method, request->uri, request->payload_length);

    if (strcmp(request->method, "GET") == 0) {
        // Find the resource with the given URI in the 'resources' array.
        size_t resource_length;
        const char *resource =
            get(request->uri, resources, MAX_RESOURCES, &resource_length);

        if (resource) {
            size_t payload_offset =
                sprintf(reply, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n",
                        resource_length);
            memcpy(reply + payload_offset, resource, resource_length);
            offset = payload_offset + resource_length;
        } else {
            reply = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            offset = strlen(reply);
        }
    } else if (strcmp(request->method, "PUT") == 0) {
        // Try to set the requested resource with the given payload in the
        // 'resources' array.
        if (set(request->uri, request->payload, request->payload_length,
                resources, MAX_RESOURCES)) {
            reply = "HTTP/1.1 204 No Content\r\n\r\n";
        } else {
            reply = "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n";
        }
        offset = strlen(reply);
    } else if (strcmp(request->method, "DELETE") == 0) {
        // Try to delete the requested resource from the 'resources' array
        if (delete (request->uri, resources, MAX_RESOURCES)) {
            reply = "HTTP/1.1 204 No Content\r\n\r\n";
        } else {
            reply = "HTTP/1.1 404 Not Found\r\n\r\n";
        }
        offset = strlen(reply);
    } else {
        reply = "HTTP/1.1 501 Method Not Supported\r\n\r\n";
        offset = strlen(reply);
    }

    // Send the reply back to the client
    if (send(conn, reply, offset, 0) == -1) {
        perror("send");
        close(conn);
    }
}


/**
 * Processes an incoming packet from the client.
 *
 * @param conn The socket descriptor representing the connection to the client.
 * @param buffer A pointer to the incoming packet's buffer.
 * @param n The size of the incoming packet.
 *
 * @return Returns the number of bytes processed from the packet.
 *         If the packet is successfully processed and a reply is sent, the
 * return value indicates the number of bytes processed. If the packet is
 * malformed or an error occurs during processing, the return value is -1.
 *
 */
ssize_t process_packet(int conn, char *buffer, size_t n, uint16_t curr_node_id, struct node_addr pred_node, struct node_addr succ_node) {
    struct request request = {
        .method = NULL, .uri = NULL, .payload = NULL, .payload_length = -1};
    ssize_t bytes_processed = parse_request(buffer, n, &request);

    if (bytes_processed > 0) {
        uint16_t uri_hash = pseudo_hash((unsigned char*) request.uri, strlen(request.uri));

        int is_responsible = check_is_responsible(curr_node_id, pred_node, uri_hash, succ_node);

        if (is_responsible == 0) {
            send_reply(conn, &request);
        }
        if (is_responsible == 1) {
            char redir_res[HTTP_MAX_SIZE];
            snprintf(redir_res, sizeof(redir_res),
                     "HTTP/1.1 303 See Other\r\n"
                     "Location: http://%s:%d%s\r\n"
                     "Content-Length: 0\r\n\r\n",
                     inet_ntoa(succ_node.node_ip), succ_node.node_port, request.uri);
            send(conn, redir_res, strlen(redir_res), 0);
        }
        // if (is_responsible == -1) {
        //     const char service_unavailable_res[] =
        //             "HTTP/1.1 503 Service Unavailable\r\n"
        //             "Retry-After: 1\r\n"
        //             "Content-Length: 0\r\n\r\n";
        //     send(conn, service_unavailable_res, strlen(service_unavailable_res), 0);
        // }

        // Check the "Connection" header in the request to determine if the
        // connection should be kept alive or closed.
        const string connection_header = get_header(&request, "Connection");
        if (connection_header && strcmp(connection_header, "close")) {
            return -1;
        }
    } else if (bytes_processed == -1) {
        // If the request is malformed or an error occurs during processing,
        // send a 400 Bad Request response to the client.
        const string bad_request = "HTTP/1.1 400 Bad Request\r\n\r\n";
        send(conn, bad_request, strlen(bad_request), 0);
        printf("Received malformed request, terminating connection.\n");
        close(conn);
        return -1;
    }

    return bytes_processed;
}

/**
 * Sets up the connection state for a new socket connection.
 *
 * @param state A pointer to the connection_state structure to be initialized.
 * @param sock The socket descriptor representing the new connection.
 *
 */
static void connection_setup(struct connection_state *state, int sock) {
    // Set the socket descriptor for the new connection in the connection_state
    // structure.
    state->sock = sock;

    // Set the 'end' pointer of the state to the beginning of the buffer.
    state->end = state->buffer;

    // Clear the buffer by filling it with zeros to avoid any stale data.
    memset(state->buffer, 0, HTTP_MAX_SIZE);
}

/**
 * Discards the front of a buffer
 *
 * @param buffer A pointer to the buffer to be modified.
 * @param discard The number of bytes to drop from the front of the buffer.
 * @param keep The number of bytes that should be kept after the discarded
 * bytes.
 *
 * @return Returns a pointer to the first unused byte in the buffer after the
 * discard.
 * @example buffer_discard(ABCDEF0000, 4, 2):
 *          ABCDEF0000 ->  EFCDEF0000 -> EF00000000, returns pointer to first 0.
 */
char *buffer_discard(char *buffer, size_t discard, size_t keep) {
    memmove(buffer, buffer + discard, keep);
    memset(buffer + keep, 0, discard); // invalidate buffer
    return buffer + keep;
}

/**
 * Handles incoming connections and processes data received over the socket.
 *
 * @param state A pointer to the connection_state structure containing the
 * connection state.
 * @return Returns true if the connection and data processing were successful,
 * false otherwise. If an error occurs while receiving data from the socket, the
 * function exits the program.
 */
bool handle_connection(struct connection_state *state, uint16_t curr_node_id, struct node_addr pred_node, struct node_addr succ_node) {
    // Calculate the pointer to the end of the buffer to avoid buffer overflow
    const char *buffer_end = state->buffer + HTTP_MAX_SIZE;

    // Check if an error occurred while receiving data from the socket
    ssize_t bytes_read =
        recv(state->sock, state->end, buffer_end - state->end, 0);
    if (bytes_read == -1) {
        perror("recv");
        close(state->sock);
        exit(EXIT_FAILURE);
    } else if (bytes_read == 0) {
        return false;
    }

    char *window_start = state->buffer;
    char *window_end = state->end + bytes_read;

    ssize_t bytes_processed = 0;
    while ((bytes_processed = process_packet(state->sock, window_start,
                                             window_end - window_start, curr_node_id, pred_node, succ_node)) > 0) {
        window_start += bytes_processed;
    }
    if (bytes_processed == -1) {
        return false;
    }

    state->end = buffer_discard(state->buffer, window_start - state->buffer,
                                window_end - window_start);
    return true;
}



/**
 * Derives a sockaddr_in structure from the provided host and port information.
 *
 * @param host The host (IP address or hostname) to be resolved into a network
 * address.
 * @param port The port number to be converted into network byte order.
 *
 * @return A sockaddr_in structure representing the network address derived from
 * the host and port.
 */
static struct sockaddr_in derive_sockaddr(const char *host, const char *port) {
    struct addrinfo hints = {
        .ai_family = AF_INET,
    };
    struct addrinfo *result_info;

    // Resolve the host (IP address or hostname) into a list of possible
    // addresses.
    int returncode = getaddrinfo(host, port, &hints, &result_info);
    if (returncode) {
        fprintf(stderr, "Error parsing host/port");
        exit(EXIT_FAILURE);
    }

    // Copy the sockaddr_in structure from the first address in the list
    struct sockaddr_in result = *((struct sockaddr_in *)result_info->ai_addr);

    // Free the allocated memory for the result_info
    freeaddrinfo(result_info);
    return result;
}

/**
 * Sets up a TCP server socket and binds it to the provided sockaddr_in address.
 *
 * @param addr The sockaddr_in structure representing the IP address and port of
 * the server.
 *
 * @return The file descriptor of the created TCP server socket.
 */
static int setup_server_socket(struct sockaddr_in addr) {
    const int enable = 1;
    const int backlog = 1;

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Avoid dead lock on connections that are dropped after poll returns but
    // before accept is called
    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // Set the SO_REUSEADDR socket option to allow reuse of local addresses
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) ==
        -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind socket to the provided address
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Start listening on the socket with maximum backlog of 1 pending
    // connection
    if (listen(sock, backlog)) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return sock;
}

static int setup_udp_socket(struct sockaddr_in addr) {
    const int enable = 1;

    // Create a socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Avoid dead lock on connections that are dropped after poll returns but
    // before accept is called
    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // Bind socket to the provided address
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    return sock;
}


void handle_udp_msg(char *buffer, struct udp_msg *msg, uint16_t curr_node_id, struct node_addr pred_node, struct node_addr succ_node, int udp_socket, struct sockaddr_in *addr) {
    memcpy(buffer, msg, sizeof(struct udp_msg));
    printf("Received UDP message: %s\n", buffer);
}


void lookup(struct udp_msg *msg, struct node_addr succ_node, int udp_socket) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(succ_node.node_port);
    addr.sin_addr = succ_node.node_ip;

    if (sendto(udp_socket, msg, sizeof(struct udp_msg), 0, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("sendto");
    } else {
        printf("LOOKUP sent to SUCC_NODE: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    }
}

bool handle_udp(int udp_socket, uint16_t curr_node_id, struct node_addr pred_node, struct node_addr succ_node) {
    char buffer[HTTP_MAX_SIZE];
    struct sockaddr_in addr;
    size_t bytes_received = recvfrom(udp_socket, buffer, sizeof(struct udp_msg), 0, (struct sockaddr *)&addr, (socklen_t *)&addr);

    if (bytes_received == -1) {
        perror("recv");
        return false;
    }

    buffer[bytes_received] = '\0';

    struct udp_msg msg;

    handle_udp_msg(buffer, &msg, curr_node_id, pred_node, succ_node, udp_socket, &addr);

    printf("  msg_type: %d\n", msg.msg_type);
    printf("  hash_id: %d\n", msg.hash_id);
    printf("  node_id: %d\n", msg.node_id);
    printf("  node_ip: %s\n", inet_ntoa(msg.node_ip));
    printf("  node_port: %d\n", msg.node_port);

    if (msg.msg_type == 0) {
        printf("LOOKUP...\n");
        lookup(&msg, succ_node, udp_socket);
    } else if (msg.msg_type == 1) {
        printf("REPLY...\n");
    } else {
        printf("Error...\n");
    }

    return true;
}


struct node_addr get_pred_node_addr() {
    struct node_addr addr = {0};

    const char *id = getenv("PRED_ID");
    const char *ip = getenv("PRED_IP");
    const char *port = getenv("PRED_PORT");

    if (!id || !ip || !port) {
        fprintf(stderr, "Umgebungsvariablen für PRED_NODE nicht gesetzt!\n");
        return addr;
    }

    addr.node_id = (uint16_t)atoi(id);
    inet_aton(ip, &addr.node_ip);
    addr.node_port = (uint16_t)atoi(port);

    return addr;
}

struct node_addr get_succ_node_addr() {
    struct node_addr addr = {0};

    const char *id = getenv("SUCC_ID");
    const char *ip = getenv("SUCC_IP");
    const char *port = getenv("SUCC_PORT");

    if (!id || !ip || !port) {
        fprintf(stderr, "Umgebungsvariablen für PRED_NODE nicht gesetzt!\n");
        return addr;
    }

    addr.node_id = (uint16_t)atoi(id);
    inet_aton(ip, &addr.node_ip);
    addr.node_port = (uint16_t)atoi(port);

    return addr;
}


/**
 *  The program expects 3; otherwise, it returns EXIT_FAILURE.
 *
 *  Call as:
 *
 *  ./build/webserver self.ip self.port
 */
int main(int argc, char **argv) {
    if (argc < 3 || argc > 4) {
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr = derive_sockaddr(argv[1], argv[2]);

    struct node_addr pred_node = get_pred_node_addr();
    struct node_addr succ_node = get_succ_node_addr();

    uint16_t curr_node_id = 0;
    if (argc == 4) {
        curr_node_id = (uint16_t)atoi(argv[3]);
    }

    printf("CURRENT_NODE_ID: %d\n", curr_node_id);
    printf("PRED: ID:%d, IP:%s, Port:%d\n", pred_node.node_id, inet_ntoa(pred_node.node_ip), pred_node.node_port);
    printf("SUCC: ID:%d, IP:%s, Port:%d\n", succ_node.node_id, inet_ntoa(succ_node.node_ip), succ_node.node_port);

    // Set up a TCP and UDP server socket.
    int server_socket = setup_server_socket(addr);
    int udp_socket = setup_udp_socket(addr);

    // Create an array of pollfd structures to monitor sockets.
    struct pollfd sockets[2] = {
        {.fd = server_socket, .events = POLLIN},
        {.fd = udp_socket, .events = POLLIN},
    };

    struct connection_state state = {0};
    while (true) {

        // Use poll() to wait for events on the monitored sockets.
        int ready = poll(sockets, sizeof(sockets) / sizeof(sockets[0]), -1);
        if (ready == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        // Process events on the monitored sockets.
        for (size_t i = 0; i < sizeof(sockets) / sizeof(sockets[0]); i += 1) {
            if (sockets[i].revents != POLLIN) {
                // If there are no POLLIN events on the socket, continue to the
                // next iteration.
                continue;
            }
            int s = sockets[i].fd;

            if (s == server_socket) {

                // If the event is on the server_socket, accept a new connection
                // from a client.
                int connection = accept(server_socket, NULL, NULL);
                if (connection == -1 && errno != EAGAIN &&
                    errno != EWOULDBLOCK) {
                    close(server_socket);
                    perror("accept");
                    exit(EXIT_FAILURE);
                } else {
                    connection_setup(&state, connection);

                    // limit to one connection at a time
                    sockets[0].events = 0;
                    sockets[1].fd = connection;
                    sockets[1].events = POLLIN;
                }
            }

            else if (sockets[i].fd == udp_socket) {
                handle_udp(udp_socket, curr_node_id, pred_node, succ_node);
            }

            else {
                assert(s == state.sock);

                // Call the 'handle_connection' function to process the incoming
                // data on the socket.
                bool cont = handle_connection(&state, curr_node_id, pred_node, succ_node);
                if (!cont) { // get ready for a new connection
                    sockets[0].events = POLLIN;
                    sockets[1].fd = -1;
                    sockets[1].events = 0;
                }
            }
        }

    }

    return EXIT_SUCCESS;
}
