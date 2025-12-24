#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "skiplist.h"

using namespace std;
using namespace std::chrono;

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
    vector<int> sizes = {100, 500, 1000, 5000, 8000, 10000, 30000};

    cout << "scale\tinsertion time(ms)\tsearch time(ms)\tdeletion time(ms)"
         << endl;

    for (int n : sizes) {
        string filename = "test_cases/test_" + to_string(n) + ".txt";
        vector<int> data = readData(filename);

        if (data.empty()) {
            continue;
        }

        SkipList sl;

        auto start = high_resolution_clock::now();
        for (int val : data) {
            sl.insert(val);
        }
        auto stop = high_resolution_clock::now();
        auto insert_duration = duration_cast<microseconds>(
            stop - start);

        start = high_resolution_clock::now();
        for (int val : data) {
            sl.search(val);
        }
        stop = high_resolution_clock::now();
        auto search_duration = duration_cast<microseconds>(
            stop - start);

        start = high_resolution_clock::now();
        for (int val : data) {
            sl.remove(val);
        }
        stop = high_resolution_clock::now();
        auto delete_duration = duration_cast<microseconds>(
            stop - start);

        cout << n << "\t"
             << insert_duration.count() / 1000.0 << "\t"
             << search_duration.count() / 1000.0 << "\t"
             << delete_duration.count() / 1000.0 << endl;
    }

    return 0;
}
