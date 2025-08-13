#include "../Utils.hpp"
#include <iostream>
#include <sstream>

using namespace std;

extern int tests_run;
extern int tests_passed;

#define TEST(name) \
    void test_##name(); \
    void run_test_##name() { \
        cout << "Running " << #name << "... "; \
        tests_run++; \
        try { \
            test_##name(); \
            cout << "PASSED" << endl; \
            tests_passed++; \
        } catch (const exception& e) { \
            cout << "FAILED: " << e.what() << endl; \
        } catch (...) { \
            cout << "FAILED: Unknown exception" << endl; \
        } \
    } \
    void test_##name()

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        ostringstream oss; \
        oss << "Expected " << (expected) << " but got " << (actual); \
        throw runtime_error(oss.str()); \
    }

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw runtime_error("Assertion failed: " #condition); \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        throw runtime_error("Assertion failed: not(" #condition ")"); \
    }

// Utils Tests
TEST(format_number_basic) {
    ASSERT_EQ("0", format_number(0));
    ASSERT_EQ("1", format_number(1));
    ASSERT_EQ("10", format_number(10));
    ASSERT_EQ("100", format_number(100));
}

TEST(format_number_thousands) {
    ASSERT_EQ("1,000", format_number(1000));
    ASSERT_EQ("10,000", format_number(10000));
    ASSERT_EQ("100,000", format_number(100000));
    ASSERT_EQ("1,000,000", format_number(1000000));
}

TEST(format_number_complex) {
    ASSERT_EQ("1,234", format_number(1234));
    ASSERT_EQ("12,345", format_number(12345));
    ASSERT_EQ("123,456", format_number(123456));
    ASSERT_EQ("1,234,567", format_number(1234567));
}

TEST(format_number_negative) {
    ASSERT_EQ("-1", format_number(-1));
    ASSERT_EQ("-1,000", format_number(-1000));
    ASSERT_EQ("-1,234,567", format_number(-1234567));
}

TEST(spaces_function) {
    ASSERT_EQ("", spaces(0));
    ASSERT_EQ(" ", spaces(1));
    ASSERT_EQ("     ", spaces(5));
    ASSERT_EQ("", spaces(-1)); // Should handle negative gracefully
}

TEST(validate_indices_valid) {
    vector<int> valid_indices = {1, 2, 3, 4, 5};
    ASSERT_TRUE(validate_indices(valid_indices, 5));
    
    vector<int> single_index = {3};
    ASSERT_TRUE(validate_indices(single_index, 5));
}

TEST(validate_indices_invalid) {
    vector<int> invalid_low = {0, 1, 2}; // 0 is invalid
    ASSERT_FALSE(validate_indices(invalid_low, 5));
    
    vector<int> invalid_high = {1, 2, 6}; // 6 is too high for max 5
    ASSERT_FALSE(validate_indices(invalid_high, 5));
    
    vector<int> mixed_invalid = {0, 3, 6};
    ASSERT_FALSE(validate_indices(mixed_invalid, 5));
}

TEST(clean_indices_remove_duplicates) {
    vector<int> duplicates = {1, 2, 2, 3, 1, 4};
    vector<int> cleaned = clean_indices(duplicates);
    vector<int> expected = {1, 2, 3, 4};
    
    ASSERT_EQ(expected.size(), cleaned.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], cleaned[i]);
    }
}

TEST(clean_indices_already_sorted) {
    vector<int> sorted = {1, 2, 3, 4};
    vector<int> cleaned = clean_indices(sorted);
    
    ASSERT_EQ(sorted.size(), cleaned.size());
    for (size_t i = 0; i < sorted.size(); ++i) {
        ASSERT_EQ(sorted[i], cleaned[i]);
    }
}

TEST(clean_indices_empty) {
    vector<int> empty;
    vector<int> cleaned = clean_indices(empty);
    ASSERT_TRUE(cleaned.empty());
}

TEST(random_int_range) {
    seed_random();
    
    // Test that random_int returns values in range
    for (int i = 0; i < 100; ++i) {
        int value = random_int(1, 10);
        ASSERT_TRUE(value >= 1);
        ASSERT_TRUE(value <= 10);
    }
    
    // Test single value range
    int single = random_int(5, 5);
    ASSERT_EQ(5, single);
}

TEST(print_cards_with_indices_output) {
    vector<Card> cards = {
        Card(ACE, SPADES),
        Card(KING, HEARTS)
    };
    
    // Redirect cout to stringstream for testing
    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());
    
    print_cards_with_indices(cards);
    
    // Restore cout
    cout.rdbuf(old_cout);
    
    string output = buffer.str();
    // Check for ASCII card elements
    ASSERT_TRUE(output.find("┌─────┐") != string::npos); // Card border
    ASSERT_TRUE(output.find("♠") != string::npos); // Spade symbol
    ASSERT_TRUE(output.find("♥") != string::npos); // Heart symbol  
    ASSERT_TRUE(output.find("1") != string::npos); // Card number 1
    ASSERT_TRUE(output.find("2") != string::npos); // Card number 2
}

TEST(print_cards_with_indices_empty) {
    vector<Card> empty_cards;
    
    // Redirect cout to stringstream for testing
    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());
    
    print_cards_with_indices(empty_cards);
    
    // Restore cout
    cout.rdbuf(old_cout);
    
    string output = buffer.str();
    ASSERT_TRUE(output.find("No cards") != string::npos);
}

// Run all utils tests
void run_utils_tests() {
    cout << "=== Utils Tests ===" << endl;
    
    run_test_format_number_basic();
    run_test_format_number_thousands();
    run_test_format_number_complex();
    run_test_format_number_negative();
    run_test_spaces_function();
    run_test_validate_indices_valid();
    run_test_validate_indices_invalid();
    run_test_clean_indices_remove_duplicates();
    run_test_clean_indices_already_sorted();
    run_test_clean_indices_empty();
    run_test_random_int_range();
    run_test_print_cards_with_indices_output();
    run_test_print_cards_with_indices_empty();
    
    cout << "Utils tests completed" << endl;
}