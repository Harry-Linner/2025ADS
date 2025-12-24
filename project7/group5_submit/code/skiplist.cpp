#include "skiplist.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

Node::Node(int val, int level) : value(val), forward(level, nullptr) {}

SkipList::SkipList(int maxL) : maxlevel(maxL), level(0) {
    header = new Node(0, maxlevel);
    srand(time(nullptr));
}

SkipList::~SkipList() {
    cleanup();
}

int SkipList::randomLevel() {
    int lvl = 1;
    while (lvl < maxlevel - 1 && 1.0 * rand() / RAND_MAX < P) {
        lvl++;
    }
    return lvl;
}

void SkipList::cleanup() {
    Node* current = header->forward[0];
    while (current != nullptr) {
        Node* next = current->forward[0];
        delete current;
        current = next;
    }
    delete header;
    header = nullptr;
}

bool SkipList::search(int val) {
    Node* current = header;
    for (int i = level - 1; i >= 0; i--) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->value < val) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    return current != nullptr && current->value == val;
}

void SkipList::insert(int val) {
    Node* current = header;
    vector<Node*> update(maxlevel, nullptr);

    for (int i = level - 1; i >= 0; i--) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->value < val) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != nullptr && current->value == val) {
        return;
    }

    int newlevel = randomLevel();

    if (newlevel > level) {
        for (int i = level; i < newlevel; i++) {
            update[i] = header;
        }
        level = newlevel;
    }

    Node* newNode = new Node(val, newlevel);
    for (int i = 0; i < newlevel; i++) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
}

void SkipList::remove(int val) {
    Node* current = header;
    vector<Node*> update(maxlevel, nullptr);

    for (int i = level - 1; i >= 0; i--) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->value < val) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != nullptr && current->value == val) {
        for (int i = 0; i < level; i++) {
            if (update[i]->forward[i] == current) {
                update[i]->forward[i] = current->forward[i];
            }
        }
        delete current;

        while (level > 1 && header->forward[level - 1] == nullptr) {
            level--;
        }
    }
}

void SkipList::display() {
    cout << "\n--- Skip List Display (Current Highest Level: "
         << level << ") ---" << endl;
    for (int i = level - 1; i >= 0; i--) {
        cout << "Level " << i << ": ";
        Node* current = header->forward[i];
        while (current != nullptr) {
            cout << current->value;
            if (i == 0) {
                cout << "(L" << current->forward.size() << ")";
            }
            cout << " -> ";
            current = current->forward[i];
        }
        cout << "NULL" << endl;
    }
    cout << "------------------------------------------" << endl;
}
