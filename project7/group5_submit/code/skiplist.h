#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <vector>

using namespace std;

#define MAX_LEVEL 16
#define P 0.5

struct Node {
    int value;
    vector<Node*> forward;

    Node(int val, int level);
};

class SkipList {
private:
    int maxlevel;
    int level;
    Node* header;

    int randomLevel();

public:
    SkipList(int maxL = MAX_LEVEL);
    ~SkipList();

    void cleanup();
    bool search(int val);
    void insert(int val);
    void remove(int val);
    void display();
};

#endif
