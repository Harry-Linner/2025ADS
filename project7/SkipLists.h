#ifndef SKIPLISTS
#define SKIPLISTS

#include<iostream>
#include<ctime>
#include<vector>
#include<cstdlib>
#include<climits>

using namespace std;

// Maximum number of levels in the skip list
#define MAX_LEVEL 16
// Probability factor for determining new node levels
#define P 0.5

// Skip list node structure
struct Node {
    int value;  // Using specific int type
    // Array of pointers to next nodes at each level
    vector<Node*> forward;
    
    // Constructor initializes value and pointer array size
    Node (int val, int level) : value(val), forward(level, nullptr) {}
};

// Skip list class definition
class SkipList {
private:
    int maxlevel; // Maximum allowed level
    int level;    // Current highest level
    Node* header; // Header node (does not store actual data)

    // Randomly generate level for new node
    int randomLevel () {
        int lvl = 1;
        while (lvl < maxlevel - 1 && 1.0 * rand() / RAND_MAX < P) {
            lvl++;
        }
        return lvl;
    } 
public:
    // Constructor: initialize header node and random seed
    SkipList (int maxL = MAX_LEVEL) : maxlevel(maxL), level(0) {
        // Create header node with default value and maximum level pointer array
        header = new Node(0, maxlevel);
    }

    // Cleanup function: release all node memory
    void cleanup() {
        Node* current = header->forward[0];
        while (current != nullptr) {
            Node* next = current->forward[0]; // Save next node before deletion
            delete current;
            current = next;
        }
        delete header; // Delete header node
        header = nullptr; // Prevent dangling pointer
    }

    // Search for value in skip list
    bool search (int val) {
        Node* current = header;
        // Start searching from current highest level
        for (int i = level - 1; i >= 0; i--) {
            // Move right at current level until next node is greater than target or null
            while (current->forward[i] != nullptr && current->forward[i]->value < val) {
                current = current->forward[i];
            }
        }
        // Move to actual node at level 0
        current = current->forward[0];
        return current != nullptr && current->value == val;
    }

    // Insert new value into skip list
    void insert (int val) {
        Node* current = header;
        // update array records node pointers that need to be updated at each level
        vector<Node*> update(maxlevel, nullptr);

        // 1. Find insertion position and record path
        for (int i = level - 1; i >= 0; i--) {
            while(current->forward[i] != nullptr && current->forward[i]->value < val) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        // Check if value already exists
        current = current->forward[0];
        if (current != nullptr && current->value == val) {
            cout << "Value " << val << " already exists, ignoring insertion." << endl;
            return;
        }

        // 2. Determine level for new node
        int newlevel = randomLevel();
        
        // If new level is higher than current highest level, update relevant pointers
        if (newlevel > level) {
            for (int i = level; i < newlevel; i++) {
                update[i] = header; // New level starts from header node
            }
            level = newlevel;
        }

        // 3. Create and link new node
        Node* newNode = new Node(val, newlevel);
        for (int i = 0; i < newlevel; i++) {
            // New node points to node originally pointed to by update[i]
            newNode->forward[i] = update[i]->forward[i]; 
            // update[i] points to new node
            update[i]->forward[i] = newNode;
        }
        cout << "Value " << val << " inserted at level " << newlevel - 1 << endl;
    }

    // Remove value from skip list
    void remove (int val) {
        Node* current = header;
        vector<Node*> update(maxlevel, nullptr);
        
        // 1. Find node to delete and record path
        for (int i = level - 1; i >= 0; i--) {
            while(current->forward[i] != nullptr && current->forward[i]->value < val) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];
        
        // 2. If node is found
        if (current != nullptr && current->value == val) {
            // Disconnect node at all levels
            for (int i = 0; i < level; i++) {
                if (update[i]->forward[i] == current) {
                    // Bypass current node
                    update[i]->forward[i] = current->forward[i];
                }
            }
            delete current;
            
            // 3. Update current highest level of skip list
            while (level > 1 && header->forward[level - 1] == nullptr) {
                level--;
            }
            cout << "Value " << val << " deleted." << endl;
        } else {
            cout << "Value " << val << " does not exist, ignoring deletion." << endl;
            return;
        }
    }

    // Display skip list structure
    void display() {
        cout << "\n--- Skip List Display (Current Highest Level: " << level << ") ---" << endl;
        for (int i = level - 1; i >= 0; i--) {
            cout << "Level " << i << ": ";
            Node* current = header->forward[i];
            while (current != nullptr) {
                // At level 0, also show total layers of node
                cout << current->value << (i == 0 ? "(L" : "") << (i == 0 ? to_string(current->forward.size()) + ")" : "") << " -> ";
                current = current->forward[i];
            }
            cout << "NULL" << endl;
        }
        cout << "------------------------------------------" << endl;
    }
};

#endif