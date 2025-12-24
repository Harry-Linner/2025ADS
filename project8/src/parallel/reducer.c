#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UNIQUE_WORDS 500000
#define MAX_WORD_LEN 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordFreq;

WordFreq results[MAX_UNIQUE_WORDS];
int result_count = 0;

/**
 * Comparison function for final output sorting.
 * 1. Count DESC
 * 2. Alphabetical ASC
 */
int compare(const void *a, const void *b) {
    WordFreq *w1 = (WordFreq *)a;
    WordFreq *w2 = (WordFreq *)b;
    if (w1->count != w2->count) {
        return w2->count - w1->count;
    }
    return strcmp(w1->word, w2->word);
}

int main() {
    char current_word[MAX_WORD_LEN] = "";
    char word_buffer[MAX_WORD_LEN];
    int count;
    int current_sum = 0;

    // Read from stdin: word[TAB]count
    // Hadoop ensures that all occurrences of the same word are grouped together
    while (scanf("%s\t%d", word_buffer, &count) != EOF) {
        if (strcmp(current_word, word_buffer) == 0) {
            current_sum += count;
        } else {
            // New word encountered, save the previous one
            if (strlen(current_word) > 0) {
                strcpy(results[result_count].word, current_word);
                results[result_count].count = current_sum;
                result_count++;
            }
            strcpy(current_word, word_buffer);
            current_sum = count;
        }
    }

    // Don't forget the last word
    if (strlen(current_word) > 0) {
        strcpy(results[result_count].word, current_word);
        results[result_count].count = current_sum;
        result_count++;
    }

    // Sort the final collected results as per assignment requirements
    qsort(results, result_count, sizeof(WordFreq), compare);

    // Print to stdout
    for (int i = 0; i < result_count; i++) {
        printf("%s %d\n", results[i].word, results[i].count);
    }

    return 0;
}