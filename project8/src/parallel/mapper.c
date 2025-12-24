#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100

/**
 * Data Cleaning: Same logic as serial program to ensure consistency.
 */
void clean_word(char *src, char *dest) {
    int j = 0;
    for (int i = 0; src[i]; i++) {
        if (isalpha(src[i])) {
            dest[j++] = tolower(src[i]);
        }
    }
    dest[j] = '\0';
}

int main() {
    char buffer[MAX_WORD_LEN];
    char cleaned[MAX_WORD_LEN];

    // Read from stdin (Hadoop feeds data here)
    while (scanf("%s", buffer) != EOF) {
        clean_word(buffer, cleaned);
        if (strlen(cleaned) > 0) {
            // Output format: word[TAB]1[NEWLINE]
            printf("%s\t1\n", cleaned);
        }
    }
    return 0;
}