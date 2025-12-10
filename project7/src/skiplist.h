typedef struct Node {
    int key;
    // The key stored in the current node
    int level;
    // The number of levels that the current key exists
    struct Node* next[];
    // Flexible Array, with the same size as level, to store the next node in different levels
} Node;

typedef struct SkipList {
    int max_level;
    // The MAXIMUM height
    int level;
    // The current highest level
    Node* head;
    // The first node
} SkipList;