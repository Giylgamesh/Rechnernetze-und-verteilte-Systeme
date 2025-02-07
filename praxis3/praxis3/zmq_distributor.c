#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>

#include "common.h"
// #include <zmq.h>


typedef struct {
    int worker_id;
    char chunk[MAX_PAYLOAD];
    void *context;
} WorkerArgs;


// DATEI IN BUFFER LESEN
// Datei öffnen
char *read_file(const char *filename, long *size) {
    FILE *file = fopen(filename, "r"); //Lesemodus
    if (!file) {
        perror("Fehler beim Datei öffnen");
        return NULL;
    }

    // Dateigröße bestimmen (Endposition)
    fseek(file, 0, SEEK_END); // Ende suchen
    *size = ftell(file); // Ende gleich Größe
    rewind(file); // Wieder zum Anfang gehen (WICHTIG!)

    // Bufferspeicher plus Nullbyte
    char *buffer = malloc((*size + 1) * sizeof(char));
    if (!buffer) {
        perror("Fehler beim Buffer allokieren");
        fclose(file); //Datei bei Problem schliessen
        return NULL;
    }

    // Dateiinhalt einlesen
    fread(buffer, 1, *size, file);
    buffer[*size] = '\0'; // Nullbyte am Ende

    fclose(file);
    return buffer; // Buffer enthält den Inhalt der Datei
}


// BUFFER IN CHUNKS UNTERTEILEN
// Funktion, um den Buffer in Chunks aufzuteilen indem ich Nullbytes hinzuzufüge
int split_file_buffer(char *buffer, long size) {
    long current_pos = 0;
    int num_chunks = 0;

    // Lesen
    while (current_pos < size) {
        // Maximal 1496 Zeichen + Nullbyte
        int max_payload_size = (current_pos + (MAX_PAYLOAD - 1) > size) ? (size - current_pos) : (MAX_PAYLOAD - 1);

        // Kein Wort abschneiden, kleiner payload
        while (current_pos +max_payload_size < size && isalpha(buffer[current_pos + max_payload_size])) {
            max_payload_size--;
        }

        // Nullbyte an das Ende des aktuellen Chunks setzen
        buffer[current_pos + max_payload_size] = '\0';

        // Ausgabe des aktuellen Chunks (zur Kontrolle)
        printf("Chunk %d: %s\n", num_chunks + 1, &buffer[current_pos]);

        // Vorbereiten für den nächsten Chunk
        current_pos += max_payload_size;
        num_chunks++;
    }

    printf("Anzahl Chunks: %d", num_chunks );
    return num_chunks;  // Anzahl der Chunks zurückgeben
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
// TODO


// CHUNK ERGEBNISSE SAMMELN
// TODO


// GESAMMELTE ERGEBNISSE AN WORKER SENDEN
// TODO


// RUECKGABE SORTIEREN
// TODO


// MAIN
int main(int argc, char *argv[]) {

    // AUSGABE DER EINGABEWERTE
    // Dateipfad
    const char *filepath = argv[1];
    printf("Dateipfad: %s\n", filepath);

    // Anzahl Arbeiter
    int num_workers =argc - 2;
    printf("%d Arbeiter\n", num_workers);

    // Ports
    for (int i = 0; i < num_workers; i++) {
        printf("Arbeiter %d auf Port %s\n", i + 1, argv[i + 2]);
    }

    // 1. DATEI VERARBEITEN
    // Datei lesen
    long file_size;
    char *file_content = read_file(argv[1], &file_size);
    if (!file_content) {
        printf("Kein Dateiinhalt\n", argv[0]);
        return 1;
    }

    // Bufferinhalt aufteilen


    // Chunks an Worker senden
    // TODO


    // Chunk Ergebnisse sammeln
    // TODO


    // Gesammelte Ergebnisse an Worker senden
    // TODO


    // Rückgabe sortieren
    // TODO

    // // Speicher freigeben
    free(file_content);
    return 0;
}
