#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>
//#include "common.h"
#include <zmq.h>

/**
 * Structs und Konstanten hinzufügen, diese sind in einer separaten Header Datei
 *
 * z.B. MAX_MSG_LENGTH
 */
#include "zmq_generics.h"

// CHUNK IN LISTE UMWANDELN
// Inhalt in Liste von Wörtern umwandeln
void chunk_to_list(const char *text, long size) {
    char word_buffer[MAX_CHUNK_SIZE]; // oben definiert
    int index = 0; // Iterator

    // Schleife iteriert durch den Buffer
    for (long i = 0; i < size; i++) {
        char c = text[i];

        if (isalpha(c)) { // Buchstabe
            word_buffer[index++] = tolower(c); // kleigeschrieben

            if (index >= MAX_CHUNK_SIZE) { // Overflow Schutz, sicherheitshalber
                word_buffer[index] = '\0';
                printf("%s\n", word_buffer);
                index = 0;
            }
        } else { // Kein Buchstabe
            if (index > 0) { // Hinter Buchstabe
                word_buffer[index] = '\0';
                printf("%s\n", word_buffer);
                index = 0;
            }
        }
    }

    // Print the last word if there is one left in the buffer
    if (index > 0) {
        word_buffer[index] = '\0';
        printf("%s\n", word_buffer);
    }
}

//  HAEUFIGKEIT DER WOERTER ZAEHLEN
// TODO


// LISTE AN DISTRIBUTOR SCHICKEN
// TODO


// ERGEBNISSE ZUSAMMENZAEHLEN
// TODO


// AN DISTRIBUTOR SCHICKEN
// TODO


// MAIN
// TODO

int main(int argc, char *argv[]) {
    if (argc < 1) {
        printf("Der Worker braucht mehr Argumente. Es fehlt wahrscheinlich der Port.");
        return EXIT_FAILURE;
    }
    char *port = argv[1];
    char worker_adress[60];
    snprintf(worker_adress, sizeof(worker_adress), "tcp://*:%s", port);

    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    zmq_bind(socket, worker_adress);

    while(1) {
        char buffer [MAX_MSG_LENGTH];
        zmq_recv (socket, buffer, MAX_MSG_LENGTH, 0);
        printf ("\nReceived Message:\n");
        printf ("%s\n", buffer);
        zmq_send (socket, "Received Message!", MAX_MSG_LENGTH, 0);

        // TODO: If request type is 'rip' then close socket
        // break;
    }

    return EXIT_SUCCESS;
}