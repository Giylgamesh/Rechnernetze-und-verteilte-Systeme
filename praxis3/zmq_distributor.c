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

char *combined_list = NULL;
size_t combined_list_size = 0;
pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;

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
        perror("Fehler beim Datei oeffnen");
        return RETURN_FAILURE;
    }

    // Dateigröße bestimmen (Endposition)
    fseek(file, 0, SEEK_END); // Ende suchen
    *size = ftell(file); // Ende gleich Größe
    rewind(file); // Wieder zum Anfang gehen (WICHTIG!)

    // Bufferspeicher plus Nullbyte '\0 == zwei bytes???'
    *fileContent = malloc((*size + 2) * sizeof(char));
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
 * @param fileContent
 * @param size
 * @return
 */
int split_words(char *fileContent, long size) {
    int num_words = 0;
    long chunk_size = 0;  // Aktuelle Chunk-Größe
    bool inside_word = false;

    for (long i = 0; i < size; i++) {
        bool isAlpha = isalpha(fileContent[i]);

        if (isAlpha) {
            if (!inside_word) {
                inside_word = true;
                num_words++;
            }
            chunk_size++;
        } else {
            if (inside_word) {
                inside_word = false;

                // Falls Chunk-Grenze erreicht oder überschritten wird: trennen
                if (chunk_size >= MAX_CHUNK_SIZE) {
                    fileContent[i] = '\0';
                    chunk_size = 0;
                }
            }
        }
    }

    //printf("\nAnzahl Worter: %d\n", num_words);
    return num_words;
}

int compare(const void *a, const void *b) {
    WordFreq *w1 = (WordFreq *)a;
    WordFreq *w2 = (WordFreq *)b;

    if (w2->frequency != w1->frequency) {
        return w2->frequency - w1->frequency;
    }
    return strcmp(w1->word, w2->word);
}

void print_output(const char *input) {
    WordFreq words[MAX_CHUNK_SIZE];
    int word_count = 0;

    int i = 0, len = strlen(input);

    while (i < len) {
        char word[MAX_CHUNK_SIZE] = {0};
        char number[10] = {0};
        int word_index = 0, num_index = 0;

        while (i < len && isalpha(input[i])) {
            word[word_index++] = input[i];
            i++;
        }
        word[word_index] = '\0';

        while (i < len && isdigit(input[i])) {
            number[num_index++] = input[i];
            i++;
        }
        number[num_index] = '\0';

        if (word_index > 0 && num_index > 0) {
            words[word_count].frequency = atoi(number);
            strcpy(words[word_count].word, word);
            word_count++;
        }
    }

    // sortieren
    qsort(words, word_count, sizeof(WordFreq), compare);

    printf("word,frequency\n"); // Uebersxhrift
    for (int j = 0; j < word_count; j++) {
        printf("%s,%d\n", words[j].word, words[j].frequency);
    }
}

/**
 *
 * Die erstellen Threads senden jeweils eine Map-Anfrage an den Worker und
 * danach nochmal eine Reduce-Anfrage mit der Antwort des Workers.
 *
 * 1. 'map' type Anfrage an den Worker mit dem Chunk im Payload
 * 2. Gemappte Antwort von Worker empfangen
 * 3. Diese Antwort wieder an den Worker senden, diesmal aber mit 'red' als type
 * 4. Nach erfolgreicher Antwort des Workers, beenden wir den Process mit einer 'rip' Anfrage
 *
 * @param argWorker
 * @return
 */
void *request_worker(void *argWorker) {
    Worker *worker = (Worker *)argWorker;

    //printf("\n[Thread] Thread erstellt. Beginne mit Verarbeitung\n");
    //printf("[Thread] Baut Verbindung zum Worker mit dem Port %s auf\n", worker->port);

    char worker_adress[60];
    snprintf(worker_adress, sizeof(worker_adress), "tcp://localhost:%s", worker->port);
    //printf("[Thread] Versuche Verbindung zum Worker aufzubauen unter: %s\n", worker_adress);

    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    if (zmq_connect(socket, worker_adress) == -1) {
        perror("[Thread] Verbindung zum Worker fehlgeschlagen");
        zmq_close(socket);
        zmq_ctx_destroy(context);
        return NULL;
    }

    //printf("[Thread] Verbindung aufgebaut zum Worker: %s\n", worker_adress);

    /**
     *
     * Erste Map-Anfrage an den Worker
     *
     */
    char request[MAX_MSG_LENGTH] = {0};
    char response[MAX_MSG_LENGTH] = {0};
    size_t chunk_position = 0;

    while (chunk_position < strlen(worker->chunk)) {
        memset(request, 0, MAX_MSG_LENGTH);
        memcpy(request, "map", 3);
        size_t chunk_size = strnlen(worker->chunk + chunk_position, MAX_MSG_LENGTH - 4);
        size_t send_size = 3 + chunk_size + 1;
        memcpy(request + 3, worker->chunk + chunk_position, chunk_size);
        request[send_size - 1] = '\0';

        if (zmq_send(socket, request, send_size, 0) == -1) {
            perror("[Thread] Senden des Request an Worker fehlgeschlagen");
            zmq_close(socket);
            zmq_ctx_destroy(context);
            return NULL;
        }
        //printf("\n[Thread] Anfrage an Worker: %s.......\n", request);

        /**
         *
         * Antwort auf die erste Map-Anfrage des Workers
         *
         */

        int recv_length = zmq_recv(socket, response, MAX_MSG_LENGTH - 1, 0);
        if (recv_length == -1) {
            perror("[Thread] Erhalten der Response vom Worker fehlgeschlagen");
            zmq_close(socket);
            zmq_ctx_destroy(context);
            return NULL;
        }
        response[recv_length] = '\0';

        //printf("[Thread] Antwort vom Worker: %s\n", response);

        /**
         *
         * Zweite Reduce-Anfrage an den Worker
         *
         */
        memset(request, 0, MAX_MSG_LENGTH);
        memcpy(request, "red", 3);
        size_t mapped_payload_size = strnlen(response, MAX_MSG_LENGTH - 4);
        size_t mapped_payload_send_size = 3 + mapped_payload_size + 1;
        memcpy(request + 3, response, mapped_payload_size);
        request[mapped_payload_send_size - 1] = '\0';

        if (zmq_send(socket, request, mapped_payload_send_size, 0) == -1) {
            perror("[Thread] Senden des Request an Worker fehlgeschlagen");
            zmq_close(socket);
            zmq_ctx_destroy(context);
            return NULL;
        }
        //printf("\n[Thread] Anfrage an Worker: %s.......\n", request);

        /**
         *
         * Antwort der zweiten Reduce-Anfrage des Worker
         *
         */
        memset(response, 0, MAX_MSG_LENGTH);
        recv_length = zmq_recv(socket, response, MAX_MSG_LENGTH - 1, 0);
        if (recv_length == -1) {
            perror("[Thread] Erhalten der Response vom Worker fehlgeschlagen");
            zmq_close(socket);
            zmq_ctx_destroy(context);
            return NULL;
        }
        response[recv_length] = '\0';

        pthread_mutex_lock(&thread_mutex);
        //
        size_t new_size = combined_list_size + recv_length + 2; // FIXME vllt. hier nochmal nachschauen bei Speicherproblemen oder falscher Ausgabe
        combined_list = realloc(combined_list, new_size);
        if (combined_list == NULL) {
            perror("Realloc für combined_list fehlgeschlagen");
            pthread_mutex_unlock(&thread_mutex);
            return NULL;
        }

        if (combined_list_size == 0) {
            combined_list[0] = '\0';
        }

        combined_list_size = new_size - 1;

        strcat(combined_list, response);
        //
        pthread_mutex_unlock(&thread_mutex);
        chunk_position += chunk_size;
    }

    /**
     *
     * Beenden des Worker-Prozesses mit einer rip-Anfrage
     *
     */
    char rip_request[4] = "rip";
    if (zmq_send(socket, rip_request, 4, 0) == -1) {
        perror("[Thread] Senden des Request an Worker fehlgeschlagen");
        zmq_close(socket);
        zmq_ctx_destroy(context);
        return NULL;
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return NULL;
}


/**
 * Main-Funktion
 *
 * @param argc Anzahl an mitgegebenen Argumenten
 * @param argv Argumente bspw. ./build/zmq_distributor testfile.txt ...
 * @return
 */
int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0); // Buffer deaktivieren
    setvbuf(stderr, NULL, _IONBF, 0);

    if (argc < 2) {
        //printf("\nEs fehlen Eingabeargumente: argv[0]: _eingabe_Datei, argv[1, ..., n] Ports von den Worker\n");
        return EXIT_FAILURE;
    }

    // Dateipfad
    const char *filepath = argv[1];
    //printf("\nDateipfad: %s\n", filepath);

    // Anzahl Worker
    int num_workers = argc - 2;
    //printf("\n%d Workers:\n", num_workers);

    // Jeder Worker erhält ein Thread
    pthread_t threads[num_workers];

    Worker *worker = calloc(num_workers, sizeof(Worker));
    memset(worker, 0, num_workers * sizeof(Worker));
    // Ports von den Worker im Worker Array initialisieren
    for (int i = 0; i < num_workers; i++) {
        worker[i].chunk = "";
        worker[i].port = argv[2 + i];
        //printf("Worker %d ist auf Port %s\n", i + 1, worker[i].port);
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
    if (read_file(filepath, &file_size, &file_content) == -1) {
        free(file_content);
        free(worker);
        return EXIT_FAILURE;
    }
    if (!file_content) {
        //printf("\n\nKein Inhalt in der Datei: %s\n", argv[0]);
        free(file_content);
        free(worker);
        return EXIT_FAILURE;
    } else {
        // //printf("\nGelesener Dateiinhalt:\n%s\n\n", file_content);
        // file_content String in Wort-Chunks 'Wort1\0Wort2\0Wort3\0' aufteilen
        // die einzelnen Chunks vom String (file_content) wird dann an verschiedene Worker verteilt
        int words_count = split_words(file_content, file_size);


        /**
         * 2. CHUNK AN WORKER SENDEN
         *
         * TODO:
         * - pthread erstellen aus dem pthread[num_workers] Array und chunks an worker aufteilen
         * - char request_worker(Worker worker[i], char chunk) Funktion erstellen
         * - auf Antwort waren
         *
         */


        if (words_count > 0) {
            int words_per_worker = words_count / num_workers;
            char *ptr = file_content;

            for (int j = 0; j < num_workers; j++) {
                worker[j].chunk = ptr; // Den Chunk in für jeden Worker speichern
                int current_word = 0;

                if (j == num_workers - 1) {
                    // Falls es der letzte Worker ist, soll er die restlichen Wörte bekommen.
                    if (ptr < file_content + file_size) {
                        while (*ptr != '\0') {
                            ptr += strlen(ptr) + 1;
                            current_word++;
                        }
                    }
                } else {
                    // Jeder andere Worker kriegt anteilig eine Anzahl an Wörter
                    while (*ptr != '\0' && current_word <= words_per_worker) {
                        ptr += strlen(ptr) + 1; // nächstes Wort
                        current_word++;
                    }
                }

                //printf("Anzahl Woerter fuer Worker[%d]: %d\n\n", j, current_word);

                if (ptr >= file_content + file_size) {
                    //printf("\nAufteilung der Woerter beendet...\n\n");
                    break;
                }
            }

            // Für jeden Worker mit dem man kommuniziert einen Thread
            for (int i = 0; i < num_workers; i++) {
                if (pthread_create(&threads[i], NULL, request_worker, &worker[i]) != 0) {
                    perror("Thread konnte nicht erstellt werden!");
                    free(file_content);
                    free(worker);
                    return EXIT_FAILURE;
                }
            }

            for (int i = 0; i < num_workers; i++) {
                pthread_join(threads[i], NULL);
            }

            if (combined_list) {
                print_output(combined_list);
                free(combined_list);
            }

        } else {
            //printf("Keine Chunks bzw. Woerter gefunden!");
            free(file_content);
            free(worker);
            return EXIT_FAILURE;
        }
    }

    // Speicher freigeben
    free(file_content);
    free(worker);
    return EXIT_SUCCESS;
}
