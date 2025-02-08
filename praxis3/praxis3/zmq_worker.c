#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include "common.h"
//#include <zmq.h>

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

// WOERTER ZAEHLEN
char* count_words(const char *input, char *output) {
    char word_buffer[MAX_PAYLOAD]; // oben definiert
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

// ZAHLEN ANPASSEN
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

// AUSGABE SCHREIBEN
// Paare bilden
typedef struct {
    char word[MAX_WORD_LEN];
    int frequency;
} WordFreq;

// Vergleichen zum Sortieren
// 1. Nach Anzahl
// 2. Alphabetisch
int compare(const void *a, const void *b) {
    WordFreq *w1 = (WordFreq *)a;
    WordFreq *w2 = (WordFreq *)b;

    if (w2->frequency != w1->frequency) {
        return w2->frequency - w1->frequency; // Nach Anzahl
    }
    return strcmp(w1->word, w2->word); // Alphabetisch
}

void print_output(const char *input) {
    WordFreq words[MAX_WORDS];  // Buffer
    int word_count = 0;

    int i = 0, len = strlen(input);

    // Parse input string
    while (i < len) {
        char word[MAX_WORD_LEN] = {0}; // Wort speichern
        char number[10] = {0};         // Zahl speichern
        int word_index = 0, num_index = 0;

        // Extract word
        while (i < len && isalpha(input[i])) {
            word[word_index++] = input[i];
            i++;
        }
        word[word_index] = '\0'; // Null

        // Anzahl rausziehen
        while (i < len && isdigit(input[i])) {
            number[num_index++] = input[i];
            i++;
        }
        number[num_index] = '\0'; // Null

        // Ins array schreiben
        if (word_index > 0 && num_index > 0) {
            words[word_count].frequency = atoi(number);
            strcpy(words[word_count].word, word);
            word_count++;
        }
    }

    // sortieren
    qsort(words, word_count, sizeof(WordFreq), compare);

    // Schreiben
    printf("word,frequency\n"); // Uebersxhrift
    for (int j = 0; j < word_count; j++) {
        printf("%s,%d\n", words[j].word, words[j].frequency);
    }
}


// Test the function
    int main() {
        char input[] = "Hello world! This is a test to test and see what words we can count and see HOW often they show up\n"
                       "C@de_Ex4mple with-symbols! and123numbers\n"
                       "UPPERcaseANDlowercase MixedCaSeWord\n"
                       "...hello..again...New---Word";
        char input2[] = "mapInteroperability test. Test uses python distributor.";

        char chunked_output[MAX_PAYLOAD];  // Buffer to store chunked words
        char counted_output[5000];         // Buffer to store counted words
        char converted_output[5000];

        // Step 1: Convert chunk to list
        chunk_to_list(input, strlen(input2)); // This prints the words

        // Step 2: Count words
        count_words(input, counted_output);

        // Step 3: Print the result
        printf("Word Count Result:\n%s\n", counted_output);

        convert_digit(counted_output, converted_output );
        printf("Result: \n%s\n", converted_output);

        print_output(converted_output);

        return 0;
}
