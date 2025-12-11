#include <iostream>
#include <string>
#include "SkipLists.h"

using namespace std;

// --- Helper function for testing search results ---
template <typename T>
void test_search(SkipList<T>& sl, T val) {
    cout << "Searching for " << val << ": " 
         << (sl.search(val) ? "Found" : "Not Found") << endl;
}

int main() {
    srand(time(0));
    // 1. Initialization and basic insertion test
    cout << "--- 1. Initializing Skip List (int) ---" << endl;
    SkipList<int> sl_int;

    // Insert a sequence of values
    sl_int.insert(15); // L1
    sl_int.insert(5);  // L2
    sl_int.insert(25); // L3
    sl_int.insert(30); // L1
    sl_int.insert(10); // L4 (Likely the highest level)
    sl_int.insert(20); // L1
    sl_int.insert(35); // L2

    // Display the initial structure
    sl_int.display();

    // 2. Search testing
    cout << "\n--- 2. Search Operations ---" << endl;
    test_search(sl_int, 10); // Should be Found
    test_search(sl_int, 25); // Should be Found
    test_search(sl_int, 17); // Should be Not Found
    test_search(sl_int, 35); // Should be Found

    // 3. Removal testing
    cout << "\n--- 3. Removal Operations ---" << endl;
    sl_int.remove(15); // Remove an existing value
    sl_int.remove(50); // Try to remove a non-existent value
    sl_int.remove(10); // Remove the highest level node (may lower the list's max level)

    // Display the structure after removals
    sl_int.display();

    // Re-verify search after removal
    cout << "\n--- 4. Final Search Check ---" << endl;
    test_search(sl_int, 15); // Should be Not Found
    test_search(sl_int, 25); // Should be Found

    // --- Optional: Test with string data type ---
    cout << "\n--- 5. Testing Skip List with Strings ---" << endl;
    SkipList<string> sl_str;
    sl_str.insert("banana");
    sl_str.insert("apple");
    sl_str.insert("date");
    sl_str.insert("cherry");
    
    sl_str.display();
    test_search(sl_str, string("apple"));

    // The destructor will be called automatically when main exits, cleaning up memory.
    cout << "\n--- Skip List test completed successfully. ---" << endl;
    return 0;
}