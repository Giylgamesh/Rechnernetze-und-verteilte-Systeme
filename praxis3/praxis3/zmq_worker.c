#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include "common.h"
//#include <zmq.h>

// CHUNK IN LISTE UMWANDELN
// Inhalt in Liste von Wörtern umwandeln
void chunk_to_list(const char *text, long size) {
    char word_buffer[MAX_PAYLOAD]; // oben definiert
    int index = 0; // Iterator

    // Schleife iteriert durch den Buffer
    for (long i = 0; i < size; i++) {
        char c = text[i];

        if (isalpha(c)) { // Buchstabe
            word_buffer[index++] = tolower(c); // kleigeschrieben

            if (index >= MAX_PAYLOAD) { // Overflow Schutz, sicherheitshalber
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