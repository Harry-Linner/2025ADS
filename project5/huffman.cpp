#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;
struct Node {
    char ch;
    int freq;
    Node *left;
    Node *right;
    Node(char c='\0', int f=0, Node *l=nullptr, Node *r=nullptr) 
    {
        ch=c;
        freq=f;
        left=l;
        right=r;
    }
    bool isLeaf() { return left==nullptr && right==nullptr;}
};
// Comparator for priority_queue to make a min-heap by frequency
struct CompareNodes 
{
    bool operator()(Node *a, Node *b)
    {return a->freq>b->freq;}// Smaller frequency gets higher priority
};
// Recursively compute code length for each character and store in huffman_lengths_map
void get_huffman_lengths(Node *root, int depth, map<char, int>& huffman_lengths_map) 
{
    if (root==nullptr) return;
    if (root->isLeaf()) 
    { // Leaf node
        huffman_lengths_map[root->ch]=depth;
        return;
    }
    get_huffman_lengths(root->left, depth + 1, huffman_lengths_map);
    get_huffman_lengths(root->right, depth + 1, huffman_lengths_map);
}
// Check whether the codes form a prefix-free code
bool is_prefix_code(const map<char, string>& student_codes) 
{
    // Check prefix property and duplicate encodings (no structured bindings for older compilers)
    for(map<char, string>::const_iterator it1=student_codes.begin();it1!=student_codes.end();++it1) {
        const char char_i=it1->first;
        const string& code_i=it1->second;
        map<char, string>::const_iterator it2=it1;
        ++it2;// start from the element right after it1
        for (;it2!=student_codes.end();++it2) 
        {
            const char char_j=it2->first;
            const string& code_j=it2->second;
            // If either code is a prefix of the other, it's not prefix-free
            if ((code_j.size()>=code_i.size() && code_j.substr(0, code_i.size())==code_i) ||
                (code_i.size()>code_j.size() && code_i.substr(0, code_j.size())==code_j))
                return false;
        }
    }
    return true;
}
int main() 
{
    int n;
    cin>>n;
    map<char,int> char_freqs;// Character frequency table
    vector<char> chars_order(n);// Original character order for later checking
    for(int i=0;i<n;i++) 
    {
        char ch;
        int freq;
        cin>>ch>>freq;
        char_freqs[ch]=freq;
        chars_order[i]=ch;// Record character
    }
    // Priority queue storing Node*,using comparator
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;
    // push nodes into the priority queue,forming a min-heap
    for(map<char, int>::const_iterator it=char_freqs.begin();it!=char_freqs.end();++it)
        pq.push(new Node(it->first,it->second));
    // Build Huffman tree
    while(pq.size()>1) //merge nodes until only one root node remains
    {
        Node *node1=pq.top();pq.pop();
        Node *node2=pq.top();pq.pop();//get two nodes with least frequency
        Node *mergedNode=new Node('\0',node1->freq+node2->freq,node1,node2);
        //create a new node with the sum of frequencies of node1 and node2,and set it as the parent of node1 and node2
        pq.push(mergedNode);
    }
    Node *huffman_root=pq.top();
    map<char, int> huffman_lengths;// record the Huffman code lengths for each character
    get_huffman_lengths(huffman_root, 0, huffman_lengths);
    long long min_huffman_total_length=0;
    for(map<char, int>::const_iterator it=char_freqs.begin();it!=char_freqs.end();++it)
        min_huffman_total_length+=(long long)it->second*huffman_lengths[it->first];//calculate the minimum total length of the Huffman codes
    int m;
    cin>>m;
    for(int i=0;i<m;i++) 
    {
        map<char, string> student_codes;
        long long current_total_length=0;
        for(int i=0;i<n;i++) 
        {
            char ch;
            string code;
            cin>>ch>>code;
            student_codes[ch]=code;
            current_total_length+=(long long)char_freqs[ch]*code.length();//calculate the total length of the input codes
        }
        bool is_prefix=is_prefix_code(student_codes);//check if there is a prefix code,which is unacceptable
        if (is_prefix && current_total_length==min_huffman_total_length)
            cout<<"Yes\n";
        else
            cout<<"No\n";
    }
    free(huffman_root);
    return 0;
}
