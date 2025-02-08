#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include "common.h"
//#include <zmq.h>

#define MAX_MSG_LEN 1500
#define MAX_WORD_LEN 1500
#define MAX_PAYLOAD 1497
#define MAX_WORKERS 12
#define MAX_WORDS 750

// CHUNK IN LISTE UMWANDELN
// Inhalt in Liste von Wörtern umwandeln
 char* chunk_to_list(const char *text, long size) {
    char word_buffer[MAX_PAYLOAD]; // oben definiert
    int index = 0; // Iterator

    // Schleife iteriert durch den Buffer
    for (long i = 0; i < size; i++) {
        char c = text[i];

        if (isalpha(c)) { // Buchstabe
            word_buffer[index++] = tolower(c); // kleingeschrieben

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
    return(word_buffer);
}

//  HAEUFIGKEIT DER WOERTER ZAEHLEN
// TODO
// Structure to store words and their counts
typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

// Wörter zaehlen
void count_words(const char *input, char *output) {
    WordCount words[MAX_WORDS];
    int word_count = 0;
    char temp[MAX_WORD_LEN];
    int temp_index = 0;

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
            if (!found && word_count < MAX_WORDS) {
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
        if (!found && word_count < MAX_WORDS) {
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

    // 2. Ausgabeformat generieren
    output[0] = '\0'; // Leeren des Output-Strings
    for (int i = 0; i < word_count; i++) {
        strcat(output, words[i].word);

        // Genau so viele Einsen anhängen, wie das Wort oft vorkommt
        for (int j = 0; j < words[i].count; j++) {
            strcat(output, "1");
        }
    }
}

// Test the function
    int main() {
        char input[] = "The example text is the best example.";
        char input2[] = "mapInteroperability test. Test uses python distributor.";

        char chunked_output[MAX_PAYLOAD];  // Buffer to store chunked words
        char counted_output[5000];         // Buffer to store counted words

        // Step 1: Convert chunk to list
        chunk_to_list(input2, strlen(input2)); // This prints the words

        // Step 2: Count words
        count_words(input2, counted_output);

        // Step 3: Print the result
        printf("Word Count Result:\n%s\n", counted_output);

        return 0;
}


// LISTE AN DISTRIBUTOR SCHICKEN
// TODO


// ERGEBNISSE ZUSAMMENZAEHLEN
// TODO


// AN DISTRIBUTOR SCHICKEN
// TODO


// MAIN
// TODO
