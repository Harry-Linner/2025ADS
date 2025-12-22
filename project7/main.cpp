#include <iostream>
#include <string>
#include <ctime>
#include "SkipLists.h"

using namespace std;

// Test search functionality
void test_search(SkipList& sl, int val) {
    cout << "Searching for " << val << ": " 
         << (sl.search(val) ? "Found" : "Not Found") << endl;
}

int main() {
    srand(time(0));
    // 1. Initialization and basic insertion test
    cout << "--- 1. Initializing Skip List (Integer Type) ---" << endl;
    SkipList sl_int;

    // Insert a series of values
    sl_int.insert(15); // L1
    sl_int.insert(5);  // L2
    sl_int.insert(25); // L3
    sl_int.insert(30); // L1
    sl_int.insert(10); // L4 (may be highest level)
    sl_int.insert(20); // L1
    sl_int.insert(35); // L2

    // Display initial structure
    sl_int.display();

    // 2. Search tests
    cout << "\n--- 2. Search Operations ---" << endl;
    test_search(sl_int, 10); // Should be found
    test_search(sl_int, 25); // Should be found
    test_search(sl_int, 17); // Should not be found
    test_search(sl_int, 35); // Should be found

    // 3. Deletion tests
    cout << "\n--- 3. Deletion Operations ---" << endl;
    sl_int.remove(15); // Delete existing value
    sl_int.remove(50); // Attempt to delete non-existent value
    sl_int.remove(10); // Delete highest level node (may reduce skip list's highest level)

    // Display structure after deletion
    sl_int.display();

    // Verify search after deletion
    cout << "\n--- 4. Final Search Check ---" << endl;
    test_search(sl_int, 15); // Should not be found
    test_search(sl_int, 25); // Should be found

    // Manually call cleanup function to free memory
    sl_int.cleanup();
    cout << "\n--- Skip List Test Completed Successfully. ---" << endl;
    return 0;
}