#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

int main() {
    // Different test scales
    vector<int> sizes = {100, 500, 1000, 5000, 8000, 10000, 30000};
    
    // Random number generator
    mt19937 gen(time(0));
    uniform_int_distribution<int> dis(1, 100000);
    
    for (int n : sizes) {
        // Create test_cases directory (if not exists)
        system("mkdir test_cases 2>nul");
        
        // Create test file
        ofstream outfile("test_cases/test_" + to_string(n) + ".txt");
        if (!outfile.is_open()) {
            cerr << "Cannot create file: test_" + to_string(n) + ".txt" << endl;
            continue;
        }
        
        // Write data scale
        outfile << n << endl;
        
        // Generate n random numbers
        for (int i = 0; i < n; i++) {
            outfile << dis(gen) << endl;
        }
        
        outfile.close();
        cout << "Generated test file: test_" + to_string(n) + ".txt (containing " << n << " elements)" << endl;
    }
    
    return 0;
}