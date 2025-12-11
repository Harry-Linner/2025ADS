#ifndef SKIPLISTS
#define SKIPLISTS

#include<iostream>
#include<ctime>
#include<vector>
#include<cstdlib>
#include<climits>

using namespace std;

#define MAX_LEVEL 16 // Maximum number of levels in the skip list
#define P 0.5        // Probability factor used to determine the level of a new node

/**
 * @brief Skip List Node Structure
 */
template <typename T>
struct Node {
    T value;
    // forward[i] is a pointer to the next node at level i
    vector<Node<T>*> forward;
    
    // Constructor initializes the value and the forward vector size
    Node (T val, int level) : value(val), forward(level, nullptr) {}
};

/**
 * @brief Skip List Class
 */
template <typename T>
class SkipList {
private:
    int maxlevel; // Maximum allowed level
    int level;    // Current highest level of the skip list
    Node<T>* header; // The header node (does not store data)

    /**
     * @brief Randomly generates a level for a new node
     * The level is determined probabilistically based on P
     * @return The randomly determined level (0 to maxlevel - 1)
     */
    int randomLevel () {
        int lvl = 1;
        while (lvl < maxlevel - 1 && 1.0 * rand() / RAND_MAX < P) {
            lvl++;
        }
        return lvl;
    } 
public:
    /**
     * @brief Constructor: initializes the header node and random seed
     */
    SkipList (int maxL = MAX_LEVEL) : maxlevel(maxL), level(0) {
        // Header node with default value T() and maxlevel forward pointers
        header = new Node<T>(T(), maxlevel);
    }

    /**
     * @brief Destructor: safely deletes all nodes to prevent memory leaks
     */
    ~SkipList () {
        Node<T>* current = header->forward[0];
        while (current != nullptr) {
            Node<T>* next = current->forward[0]; // Save the next pointer before deleting current
            delete current;
            current = next;
        }
        delete header; // Delete the header node
    }

    /**
     * @brief Searches for a value in the skip list
     * @param val The value to search for
     * @return true if the value is found, false otherwise
     */
    bool search (T val) {
        Node<T>* current = header;
        // Start from the highest current level and move down
        for (int i = level - 1; i >= 0; i--) {
            // Traverse right at the current level until next node is > val or nullptr
            while (current->forward[i] != nullptr && current->forward[i]->value < val) {
                current = current->forward[i];
            }
        }
        // Move to the actual node at level 0
        current = current->forward[0];
        return current != nullptr && current->value == val;
    }

    /**
     * @brief Inserts a new value into the skip list
     * @param val The value to insert
     */
    void insert (T val) {
        Node<T>* current = header;
        // update vector stores pointers where the link needs to be updated
        vector<Node<T>*> update (maxlevel, nullptr); 

        // 1. Find the insertion position and record the update path
        for (int i = level - 1; i >= 0; i--) {
            while(current->forward[i] != nullptr && current->forward[i]->value < val) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        // Check if value already exists
        current = current->forward[0];
        if (current != nullptr && current->value == val) {
            cout << "Value " << val << " already exists. Insertion ignored." << endl;
            return ;
        }

        // 2. Determine the level for the new node
        int newlevel = randomLevel();
        
        // If the new level is higher than the current max level, update the level
        if (newlevel > level) {
            for (int i = level; i < newlevel; i++) {
                update[i] = header; // New levels start from the header
            }
            level = newlevel;
        }

        // 3. Create and link the new node
        Node<T>* newNode = new Node<T>(val, newlevel);
        for (int i = 0; i < newlevel; i++) {
            // New node links to what update[i] was pointing to
            newNode->forward[i] = update[i]->forward[i]; 
            // update[i] links to the new node
            update[i]->forward[i] = newNode;
        }
        cout << "Value " << val << " inserted with level " << newlevel - 1 << endl;
    }

    /**
     * @brief Removes a value from the skip list
     * @param val The value to remove
     */
    void remove (T val) {
        Node<T>* current = header;
        vector<Node<T>*> update (maxlevel, nullptr);
        
        // 1. Find the node to be removed and record the update path
        for (int i = level - 1; i >= 0; i--) {
            while(current->forward[i] != nullptr && current->forward[i]->value < val) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];
        
        // 2. If the node is found
        if (current != nullptr && current->value == val) {
            // Unlink the node at all its levels
            for (int i = 0; i < level; i++) {
                if (update[i]->forward[i] == current) {
                    // Bypass the current node
                    update[i]->forward[i] = current->forward[i];
                }
            }
            delete current;
            
            // 3. Update the skip list's current level if necessary
            while (level > 1 && header->forward[level - 1] == nullptr) {
                level--;
            }
            cout << "Value " << val << " has been removed." <<endl;
        } else {
            cout << "Value " << val << " doesn't exist. Removal ignored." << endl;
            return ;
        }
    }

    /**
     * @brief Displays the skip list structure level by level
     */
    void display() {
        std::cout << "\n--- Skip List Display (Current Max Level: " << level << ") ---" << std::endl;
        for (int i = level - 1; i >= 0; i--) {
            std::cout << "Level " << i << ": ";
            Node<T>* current = header->forward[i];
            while (current != nullptr) {
                // For Level 0, also show the node's full level count (L[count])
                std::cout << current->value << (i == 0 ? "(L" : "") << (i == 0 ? std::to_string(current->forward.size()) + ")" : "") << " -> ";
                current = current->forward[i];
            }
            std::cout << "NULL" << std::endl;
        }
        std::cout << "------------------------------------------" << std::endl;
    }
};

#endif