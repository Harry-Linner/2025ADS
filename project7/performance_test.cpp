#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <chrono>
#include "SkipLists.h"

using namespace std;
using namespace std::chrono;

// Read data from file
vector<int> readData(const string& filename) {
    vector<int> data;
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return data;
    }
    
    int n, value;
    infile >> n;
    for (int i = 0; i < n; i++) {
        infile >> value;
        data.push_back(value);
    }
    
    infile.close();
    return data;
}

int main() {
    // Test different data scales
    vector<int> sizes = {100, 500, 1000, 5000, 8000, 10000, 30000};
    
    cout << "规模\t插入时间(ms)\t查找时间(ms)\t删除时间(ms)" << endl;
    
    for (int n : sizes) {
        string filename = "test_cases/test_" + to_string(n) + ".txt";
        vector<int> data = readData(filename);
        
        if (data.empty()) {
            continue;
        }
        
        SkipList sl;
        
        // Test insertion performance
        auto start = high_resolution_clock::now();
        for (int val : data) {
            sl.insert(val);
        }
        auto stop = high_resolution_clock::now();
        auto insert_duration = duration_cast<microseconds>(stop - start);
        
        // Test search performance
        start = high_resolution_clock::now();
        for (int val : data) {
            sl.search(val);
        }
        stop = high_resolution_clock::now();
        auto search_duration = duration_cast<microseconds>(stop - start);
        
        // Test deletion performance
        start = high_resolution_clock::now();
        for (int val : data) {
            sl.remove(val);
        }
        stop = high_resolution_clock::now();
        auto delete_duration = duration_cast<microseconds>(stop - start);
        
        // Output results
        cout << n << "\t" 
             << insert_duration.count()/1000.0 << "\t" 
             << search_duration.count()/1000.0 << "\t" 
             << delete_duration.count()/1000.0 << endl;
        
        // Clean up memory
        sl.cleanup();
    }
    
    return 0;
}