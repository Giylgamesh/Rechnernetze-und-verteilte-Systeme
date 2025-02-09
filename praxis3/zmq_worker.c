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
char* chunk_to_list(const char *text, long size) {
    char *word_buffer = malloc((size + 1) * sizeof(char));
    memset(word_buffer, 0, size + 1);

    int buffer_index = 0;
    int last_was_space = 1;

    for (long i = 0; i < size; i++) {
        char c = text[i];

        if (isalpha(c)) {
            word_buffer[buffer_index++] = tolower(c);
            last_was_space = 0;
        } else {
            if (!last_was_space) {
                word_buffer[buffer_index++] = ' ';
                last_was_space = 1;
            }
        }

        if (buffer_index >= size - 1) {
            break;
        }
    }

    if (buffer_index > 0 && word_buffer[buffer_index - 1] == ' ') {
        buffer_index--;
    }

    word_buffer[buffer_index] = '\0';

    return word_buffer;
}


void count_words(const char *input, char *output) {
    WordCount words[MAX_CHUNK_SIZE];
    int word_count = 0;
    char temp[MAX_CHUNK_SIZE];
    int temp_index = 0;

    //printf("\n\nCOUNTING INPUT: %s\n\n", input);

    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            temp[temp_index++] = tolower(input[i]);
        } else if (temp_index > 0) {
            temp[temp_index] = '\0';
            temp_index = 0;

            // Gibt es das Wort schon
            int found = 0;
            for (int j = 0; j < word_count; j++) {
                if (strcmp(words[j].word, temp) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            // Neues Wort
            if (!found && word_count < MAX_CHUNK_SIZE) {
                strcpy(words[word_count].word, temp);
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    // Letztes Wort
    if (temp_index > 0) {
        temp[temp_index] = '\0';
        int found = 0;
        for (int j = 0; j < word_count; j++) {
            if (strcmp(words[j].word, temp) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && word_count < MAX_CHUNK_SIZE) {
            strcpy(words[word_count].word, temp);
            words[word_count].count = 1;
            word_count++;
        }
    }

    // String aus liste erstellen
    output[0] = '\0';
    for (int i = 0; i < word_count; i++) {
        char buffer[110];
        snprintf(buffer, sizeof(buffer), "%s%d", words[i].word, words[i].count);
        strcat(output, buffer);
    }

    output[0] = '\0'; // Leeren des Output-Strings
    for (int i = 0; i < word_count; i++) {
        strcat(output, words[i].word);

        // Genau so viele Einsen anhängen, wie das Wort oft vorkommt
        for (int j = 0; j < words[i].count; j++) {
            strcat(output, "1");
        }
    }
    //printf("OUT: %s", output);
}

void convert_digit(const char *input, char *output) {
    int len = strlen(input);
    int out_index = 0;
    int i = 0;

    while (i < len) {
        output[out_index++] = input[i];  // Kopieren

        // Einsen
        if (input[i] == '1') {
            int count = 0;

            // einsen zaehlen
            while (i < len && input[i] == '1') {
                count++;
                i++;
            }

            // In zahl umwandeln
            out_index--;  // Remove the last copied '1'
            out_index += sprintf(&output[out_index], "%d", count);
        } else {
            i++;
        }
    }

    output[out_index] = '\0';  // Null-terminate output string
}

void *create_worker_thread(void *worker_adress) {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    zmq_bind(socket, worker_adress);

    printf("[Worker] Gestartet und horcht auf: %s\n", worker_adress);

    while(1) {
        char buffer[MAX_MSG_LENGTH];
        memset(buffer, 0, MAX_MSG_LENGTH);
        int recv_bytes = zmq_recv(socket, buffer, MAX_MSG_LENGTH, 0);
        if (recv_bytes == -1) {
            perror("[Worker] Erhalten der Response vom Worker fehlgeschlagen");
            return NULL;
        }
        printf("\n[Worker] Erhaltene Nachricht: %s\n", buffer);

        char type[4] = {0};
        strncpy(type, buffer, 3); // Die ersten 3 Bytes sind type
        char *payload = buffer + 3; // Payload
        long payload_length = MAX_MSG_LENGTH - 3;

        char response[MAX_MSG_LENGTH] = {0};

        if (strcmp(type, "map") == 0) {
            printf("[Worker] MAP wird ausgeführt...\n");

            /**
             *
             */
            char *word_list = chunk_to_list(payload, payload_length);
            printf("[Worker] Wortliste nach Chunking: %lu\n", strlen(word_list));

            /**
             *
             */

            count_words(word_list, response);
            printf("[Worker] Wortliste nach Counting: %lu\n", strlen(response));

            /**
             *
             */
            zmq_send(socket, response, strlen(response) + 1, 0);
            printf("[Worker] Antwort Bytes an Distributor gesendet: %lu\n", strlen(response));
            free(word_list);
        } else if (strcmp(type, "red") == 0) {
            printf("REDUCE wird ausgeführt...\n");

            memset(response, 0, MAX_MSG_LENGTH);
            convert_digit(payload, response);

            zmq_send(socket, response, strlen(response) + 1, 0);
            printf("[Worker] Antwort an Distributor gesendet: %lu\n", strlen(response));

        } else if (strcmp(type, "rip") == 0) {
            printf("[Worker] wird heruntergefahren...\n");
            memset(response, 0, MAX_MSG_LENGTH);
            memcpy(response, "rip", 3);
            zmq_send(socket, response, strlen(response) + 1, 0);
            break;

        } else {
            printf("[Worker] Unbekannter Befehl: %s\n", type);
            strcpy(response, "Fehler: Unbekannter Befehl");
            zmq_send(socket, response, strlen(response) + 1, 0);
        }


    }

    zmq_close(socket);
    zmq_ctx_destroy(context);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc < 1) {
        printf("Der Worker braucht mehr Argumente. Es fehlt wahrscheinlich der Port.");
        return EXIT_FAILURE;
    }

    int num_workers = argc - 1;
    pthread_t threads[num_workers];
    printf("\n%d Worker-Threads:\n", num_workers);

    WorkerAdressList worker_addresses[num_workers];

    // Ports von den Worker im WorkerAddress-Array initialisieren
    for (int i = 0; i < num_workers; i++) {
        printf("Worker %d ist auf Port %s\n", i + 1, argv[1 + i]);

        worker_addresses[i].address = malloc(60);
        snprintf(worker_addresses[i].address, 60, "tcp://*:%s", argv[1 + i]);

        printf("ADRESS TO INSERT: %s\n", worker_addresses[i].address);
    }

    for (int i = 0; i < num_workers; i++) {
        if (pthread_create(&threads[i], NULL, create_worker_thread, worker_addresses[i].address) != 0) {
            perror("Thread konnte nicht erstellt werden!");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < num_workers; i++) {
        pthread_join(threads[i], NULL);
        free(worker_addresses[i].address);
    }



    return EXIT_SUCCESS;
}