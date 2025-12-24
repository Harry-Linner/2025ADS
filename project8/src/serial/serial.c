#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WORDS 500000  // Maximum number of unique words allowed
#define MAX_WORD_LEN 100  // Maximum length of a single word

// Structure to store word and its frequency
typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordFreq;

WordFreq dict[MAX_WORDS];
int dict_size = 0;

/**
 * Data Cleaning: Converts word to lowercase and removes non-alphabetic characters.
 */
void clean_word(char *src, char *dest) {
    int j = 0;
    for (int i = 0; src[i]; i++) {
        if (isalpha(src[i])) { // Keep only alphabetic characters
            dest[j++] = tolower(src[i]);
        }
    }
    dest[j] = '\0';
}

/**
 * Word Counting: Searches for the word in the dictionary. 
 * Increments count if found, otherwise adds as a new entry.
 */
void add_to_dict(char *w) {
    if (strlen(w) == 0) return;
    
    // Linear Search: This is the bottleneck of the serial program.
    // It will serve as a great point of comparison for your MapReduce analysis.
    for (int i = 0; i < dict_size; i++) {
        if (strcmp(dict[i].word, w) == 0) {
            dict[i].count++;
            return;
        }
    }
    
    // Add new word if not found
    if (dict_size < MAX_WORDS) {
        strcpy(dict[dict_size].word, w);
        dict[dict_size].count = 1;
        dict_size++;
    }
}

/**
 * Comparison function for qsort.
 * Sorting Rules:
 * 1. Primary: Frequency in non-increasing (descending) order.
 * 2. Secondary: Lexicographical order (ascending) if frequencies are equal.
 */
int compare(const void *a, const void *b) {
    WordFreq *w1 = (WordFreq *)a;
    WordFreq *w2 = (WordFreq *)b;
    
    if (w1->count != w2->count) {
        return w2->count - w1->count; // Higher count comes first
    }
    return strcmp(w1->word, w2->word); // Dictionary order (a-z)
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char buffer[MAX_WORD_LEN];
    char cleaned[MAX_WORD_LEN];
    
    printf("Starting serial processing...\n");
    clock_t start = clock(); // Start timing

    // Read and count words
    while (fscanf(file, "%s", buffer) != EOF) {
        clean_word(buffer, cleaned);
        add_to_dict(cleaned);
    }
    fclose(file);

    // Sort the results based on the assignment requirements
    qsort(dict, dict_size, sizeof(WordFreq), compare);

    clock_t end = clock(); // End timing
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // Output results: word followed by a space and its count
    // No extra space at the end of each line
    for (int i = 0; i < dict_size; i++) {
        printf("%s %d\n", dict[i].word, dict[i].count);
    }

    // Print performance metrics to stderr (to keep stdout clean for the results)
    fprintf(stderr, "\n--- Performance Metrics ---\n");
    fprintf(stderr, "Total unique words: %d\n", dict_size);
    fprintf(stderr, "Time taken: %.4f seconds\n", time_spent);

    return 0;
}