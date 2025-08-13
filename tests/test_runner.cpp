#include <iostream>

using namespace std;

// Global test counters
int tests_run = 0;
int tests_passed = 0;

// Test function declarations
void run_card_tests();
void run_poker_tests(); 
void run_joker_tests();
void run_utils_tests();
void run_memory_tests();
void run_handpreview_tests();

int main() {
    cout << "========================================" << endl;
    cout << "       BALATRO C++ TEST SUITE" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    // Reset counters
    tests_run = 0;
    tests_passed = 0;
    
    // Run all test suites
    run_card_tests();
    cout << endl;
    
    run_poker_tests();
    cout << endl;
    
    run_joker_tests();
    cout << endl;
    
    run_utils_tests();
    cout << endl;
    
    run_memory_tests();
    cout << endl;
    
    run_handpreview_tests();
    cout << endl;
    
    // Print final results
    cout << "========================================" << endl;
    cout << "FINAL RESULTS: " << tests_passed << "/" << tests_run << " tests passed";
    
    if (tests_passed == tests_run) {
        cout << " ✓" << endl;
        cout << "All tests PASSED!" << endl;
        return 0;
    } else {
        cout << " ✗" << endl;
        cout << (tests_run - tests_passed) << " tests FAILED!" << endl;
        return 1;
    }
}