#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "BST.h"
#include "AVLTree.h"
#include "splay.h"

// Macro to test insert and delete operations for a tree type
// It now outputs directly to a single file in a standardized format
#define TEST_INSERT_DELETE(tree_name_str, tree_type, root, insert_func, delete_func, values_arr, delete_arr, n, file, total_time) \
    clock_t start_time_##tree_type = clock(); \
    for (int repeat = 0; repeat < 100; repeat++) { \
        root = NULL; \
        for (int i = 0; i < n; i++) { \
            root = insert_func(root, values_arr[i]); \
        } \
        for (int i = 0; i < n; i++) { \
            root = delete_func(root, delete_arr[i]); \
        } \
        free_##tree_type(root); \
    } \
    clock_t end_time_##tree_type = clock(); \
    total_time = (double)(end_time_##tree_type - start_time_##tree_type) / CLOCKS_PER_SEC ; \
    fprintf(file, "%s,%d,%s,%f\n", tree_name_str, n, order_type, total_time); \
    printf("Performance on %s with N=%d (%s): %f seconds\n", tree_name_str, n, order_type, total_time);

// Helper function to free BST
void free_BST(BSTNode* node) {
    if (node == NULL) return;
    free_BST(node->left);
    free_BST(node->right);
    free(node);
}
// Helper function to free AVL Tree
void free_AVL(AVLNode* node) {
    if (node == NULL) return;
    free_AVL(node->left);
    free_AVL(node->right);
    free(node);
}


// Custom Splay tree insertion and deletion logic to fit the macro
SplayNode* insert_Splay(SplayNode* root, int value) {
    SplayNode* newNode = createnode(value);
    root = insert(newNode, root);
    root = splay(newNode, root);
    return root;
}

SplayNode* delete_Splay(SplayNode* root, int value) {
    SplayNode* target = search(value, root);
    if (target) {
        root = splay(target, root);
        root = delete(root);
    }
    return root;
}


// 释放 Splay Tree 节点
void free_Splay(SplayNode* node) {
    if (node == NULL) return;
    free_Splay(node->left);
    free_Splay(node->right);
    free(node);
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <N> <order_type>\n", argv[0]);
        fprintf(stderr, "  <N>: Number of integers\n");
        fprintf(stderr, "  <order_type>: 'inc', 'dec', or 'rand'\n");
        return 1;
    }

    int n = atoi(argv[1]);
    const char *order_type = argv[2];

    if (n <= 0) {
        fprintf(stderr, "Error: N must be a positive integer.\n");
        return 1;
    }

    char input_path[100];
    sprintf(input_path, "test_data/input_%d_%s.txt", n, order_type);

    FILE *input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        perror("Failed to open input file");
        return 1;
    }

    int *tempArray = (int*)calloc(n, sizeof(int));
    int *deleteArray = (int*)calloc(n, sizeof(int)); // 新增
    if (tempArray == NULL || deleteArray == NULL) {
        perror("Memory allocation failed");
        fclose(input_file);
        free(tempArray);
        free(deleteArray);
        return 1;
    }
    
    // Read data
    for (int i = 0; i < n; i++) {
        fscanf(input_file, "%d", tempArray + i);
        deleteArray[i] = tempArray[i]; // 复制
    }
    fclose(input_file);

    // 按 order_type 洗牌 deleteArray
    if(strcmp(order_type, "dec") == 0) {
        for(int i = 0; i < n / 2; i++) {
            int temp = deleteArray[i];
            deleteArray[i] = deleteArray[n - i - 1];
            deleteArray[n - i - 1] = temp;
        }
    } else if(strcmp(order_type, "rand") == 0) {
        srand(time(NULL));
        for(int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = deleteArray[i];
            deleteArray[i] = deleteArray[j];
            deleteArray[j] = temp;
        }
    }

    // Open the output file in append mode
    FILE *output_file = fopen("test_data/performance_results.txt", "a");
    if (output_file == NULL) {
        perror("Failed to open output file");
        free(tempArray);
        free(deleteArray);
        return 1;
    }

    double total_time;

    // 1. Test BST
    BSTNode *BSTroot = NULL;
    TEST_INSERT_DELETE("BST", BST, BSTroot, insertBST, deleteBST, tempArray, deleteArray, n, output_file, total_time);

    // 2. Test AVL Tree
    AVLNode *AVLroot = NULL;
    TEST_INSERT_DELETE("AVL", AVL, AVLroot, insertAVL, deleteAVL, tempArray, deleteArray, n, output_file, total_time);
    
    // 3. Test Splay Tree
    SplayNode *Splayroot = NULL;
    TEST_INSERT_DELETE("Splay", Splay, Splayroot, insert_Splay, delete_Splay, tempArray, deleteArray, n, output_file, total_time);

    free(tempArray);
    free(deleteArray);
    fclose(output_file);

    return 0;
}