#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

//#include "common.h"
#include <zmq.h>

/**
 * Structs und Konstanten hinzufügen, diese sind in einer separaten Header Datei
 *
 * z.B. MAX_MSG_LENGTH
 */
#include "zmq_generics.h"


/**
 * Öffnet die Datei und liest ihn.
 * Der Stream wird als char Buffer zurückgegeben.
 *
 * @param filename
 * @param size
 * @return
 */
int read_file(const char *filename, long *size, char **fileContent) {
    FILE *file = fopen(filename, "r"); //Lesemodus
    if (!file) {
        perror("Fehler beim Datei öffnen");
        return RETURN_FAILURE;
    }

    // Dateigröße bestimmen (Endposition)
    fseek(file, 0, SEEK_END); // Ende suchen
    *size = ftell(file); // Ende gleich Größe
    rewind(file); // Wieder zum Anfang gehen (WICHTIG!)

    // Bufferspeicher plus Nullbyte
    *fileContent = malloc((*size + 1) * sizeof(char));
    if (!*fileContent) {
        perror("Fehler bei der Speicher-Allokation");
        fclose(file);
        return RETURN_FAILURE;
    }

    // Dateiinhalt in den return Buffer einlesen
    fread(*fileContent, 1, *size, file);
    (*fileContent)[*size] = '\0'; // Nullterminator am Ende

    fclose(file);
    return EXIT_SUCCESS; // Buffer enthält den Inhalt der Datei
}


/**
 * Hier wird der Buffer in Chunks aufgeteilt, indem am Ende eines Wortes der Nullterminator angehängt wird.
 *
 * @param buffer
 * @param size
 * @return
 */
int split_words(char *buffer, long size) {
    int num_words = 0;
    bool has_word_ended = false; // Auf true setzen, wenn der in der Schleife betrachtete Zeichen kein Buchstabe ist.

    for (long i = 0; i < size; i++) {
        bool isAlpha = isalpha(buffer[i]);
        if (isAlpha) {
            if (has_word_ended) {
                num_words++;
                has_word_ended = false;
            }
        } else {
            if (!has_word_ended) {
                buffer[i] = '\0';
                has_word_ended = true;
            }
        }
    }

    // Jedes gefundene Wort untereinander ausgeben [DEBUG]
    char *ptr = buffer;
    printf("Gefundene Wörter:\n");
    while (*ptr != '\0') {
        printf("[%s]\n", ptr);
        ptr += strlen(ptr) + 1;  // nächstes Wort
    }

    printf("\nAnzahl Wörter: %d\n", num_words);
    return num_words;
}

// Chunks verteilen
/*
void distribute_chunks(char **chunks, int num_chunks, int num_workers, ChunkAssignment *assignments) {
    for (int i = 0; i < num_chunks; i++) {
        assignments[i].worker_id = i % num_workers;  // Round-Robin
        assignments[i].chunk = chunks[i];  // Pointer zum String
    }
}
*/

// CHUNKS AN WORKER SENDEN
char *request_worker(Worker worker, char chunk) {

}


// CHUNK ERGEBNISSE SAMMELN
// TODO


// GESAMMELTE ERGEBNISSE AN WORKER SENDEN
// TODO


// RUECKGABE SORTIEREN
// TODO


/**
 * Main-Funktion
 *
 * @param argc Anzahl an mitgegebenen Argumenten
 * @param argv Argumente bspw. ./build/zmq_distributor testfile.txt ...
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("\nEs fehlen Eingabeargumente: argv[0]: _eingabe_Datei, argv[1, ..., n] Ports von den Worker\n");
        return EXIT_FAILURE;
    }

    // Dateipfad
    const char *filepath = argv[1];
    printf("\nDateipfad: %s\n", filepath);

    // Anzahl Worker
    int num_workers = argc - 2;
    printf("\n%d Workers:\n\n", num_workers);

    // Jeder Worker erhält ein Thread
    pthread_t threads[num_workers];

    Worker worker[num_workers];
    // Ports von den Worker im Worker Array initialisieren
    for (int i = 0; i < num_workers; i++) {
        worker[i].chunk = NULL;
        worker[i].port = argv[2 + i];
        printf("Worker %d ist auf Port %s\n", i + 1, worker[i].port);
    }


    /**
     * 1. DATEI VERARBEITEN
     *
     * - Auslesen
     * - String in Wort-Chunks aufteilen (werden mit \0 getrennt)
     * - Chunks an Worker senden
     *
     */


    // Datei lesen und in file_content speichern
    long file_size;
    char *file_content = NULL;
    if (read_file(argv[1], &file_size, &file_content) == -1) {
        return EXIT_FAILURE;
    }
    if (!file_content) {
        printf("\n\nKein Inhalt in der Datei: %s\n", argv[0]);
        return EXIT_FAILURE;
    } else {
        printf("\n\nGelesener Dateiinhalt:\n\n%s\n\n", file_content);
        // file_content String in Wort-Chunks 'Wort1\0Wort2\0Wort3\0' aufteilen
        // die einzelnen Chunks vom String (file_content) wird dann an verschiedene Worker verteilt
        int chunks_count = split_words(file_content, file_size);


        /**
         * 2. CHUNK AN WORKER SENDEN
         *
         * TODO:
         * - pthread erstellen aus dem pthread[num_workers] Array und chunks an worker aufteilen
         * - char request_worker(Worker worker[i], char chunk)   Funktion erstellen
         * - auf Antwort waren
         *
         */


        if (chunks_count > 0) {
            printf("\n\nString an Worker: %s", file_content);
        } else {
            printf("Keine Chunks bzw. Wörter gefunden!");
            return EXIT_FAILURE;
        }
    }

    // Bufferinhalt aufteilen
    // TODO

    // Chunks an Worker senden
    // TODO


    // Chunk Ergebnisse sammeln
    // TODO


    // Gesammelte Ergebnisse an Worker senden
    // TODO


    // Rückgabe sortieren
    // TODO

    // Speicher freigeben
    free(file_content);
    return EXIT_SUCCESS;
}
