#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Helper function to generate a shuffled array
void shuffle(int *array, int n) {
    if (n > 1) {
        int i;
        for (i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <N> <order_type>\n", argv[0]);
        fprintf(stderr, "  <N>: Number of integers to generate\n");
        fprintf(stderr, "  <order_type>: 'inc' for increasing, 'dec' for decreasing, 'rand' for random\n");
        return 1;
    }

    int N = atoi(argv[1]);
    const char *order_type = argv[2];

    if (N <= 0) {
        fprintf(stderr, "N must be a positive integer.\n");
        return 1;
    }

    int *data = (int *)malloc(N * sizeof(int));
    if (data == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    // Populate data with numbers 1 to N
    for (int i = 0; i < N; i++) {
        data[i] = i + 1;
    }

    // Reorder based on the specified type
    if (strcmp(order_type, "inc") == 0) {
        // Already in increasing order, no change needed
    } else if (strcmp(order_type, "dec") == 0) {
        // Reverse for decreasing order
        for (int i = 0; i < N / 2; i++) {
            int temp = data[i];
            data[i] = data[N - 1 - i];
            data[N - 1 - i] = temp;
        }
    } else if (strcmp(order_type, "rand") == 0) {
        // Shuffle for random order
        srand((unsigned int)time(NULL));
        shuffle(data, N);
    } else {
        fprintf(stderr, "Invalid order_type. Use 'inc', 'dec', or 'rand'.\n");
        free(data);
        return 1;
    }

    // Output the data in the required format
    printf("%d\n", N); // First line is the number of elements
    for (int i = 0; i < N; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    free(data);
    return 0;
}