#include "../Card.hpp"
#include <iostream>
#include <sstream>
#include <cassert>

using namespace std;

// Simple test framework - extern declarations
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

// Card Tests
TEST(card_default_constructor) {
    Card c;
    ASSERT_EQ(TWO, c.get_rank());
    ASSERT_EQ(SPADES, c.get_suit());
}

TEST(card_parameterized_constructor) {
    Card c(KING, HEARTS);
    ASSERT_EQ(KING, c.get_rank());
    ASSERT_EQ(HEARTS, c.get_suit());
}

TEST(card_is_face_or_ace) {
    Card jack(JACK, SPADES);
    Card queen(QUEEN, HEARTS);
    Card king(KING, CLUBS);
    Card ace(ACE, DIAMONDS);
    Card ten(TEN, SPADES);
    Card two(TWO, HEARTS);
    
    ASSERT_TRUE(jack.is_face_or_ace());
    ASSERT_TRUE(queen.is_face_or_ace());
    ASSERT_TRUE(king.is_face_or_ace());
    ASSERT_TRUE(ace.is_face_or_ace());
    ASSERT_FALSE(ten.is_face_or_ace());
    ASSERT_FALSE(two.is_face_or_ace());
}

TEST(card_equality_operators) {
    Card c1(KING, HEARTS);
    Card c2(KING, HEARTS);
    Card c3(KING, SPADES);
    Card c4(QUEEN, HEARTS);
    
    ASSERT_TRUE(c1 == c2);
    ASSERT_FALSE(c1 == c3);
    ASSERT_FALSE(c1 == c4);
    
    ASSERT_FALSE(c1 != c2);
    ASSERT_TRUE(c1 != c3);
    ASSERT_TRUE(c1 != c4);
}

TEST(card_comparison_operators) {
    Card two(TWO, SPADES);
    Card three(THREE, HEARTS);
    Card king(KING, CLUBS);
    Card ace(ACE, DIAMONDS);
    
    ASSERT_TRUE(two < three);
    ASSERT_TRUE(three < king);
    ASSERT_TRUE(king < ace);
    ASSERT_FALSE(ace < two);
    
    ASSERT_TRUE(ace > king);
    ASSERT_TRUE(king > three);
    ASSERT_TRUE(three > two);
    ASSERT_FALSE(two > ace);
}

TEST(card_stream_output) {
    Card c(ACE, SPADES);
    ostringstream oss;
    oss << c;
    ASSERT_EQ("Ace of Spades", oss.str());
}

TEST(card_stream_input) {
    istringstream iss("King of Hearts");
    Card c;
    iss >> c;
    ASSERT_EQ(KING, c.get_rank());
    ASSERT_EQ(HEARTS, c.get_suit());
}

TEST(rank_string_conversion) {
    ASSERT_EQ(TWO, string_to_rank("Two"));
    ASSERT_EQ(JACK, string_to_rank("Jack"));
    ASSERT_EQ(ACE, string_to_rank("Ace"));
}

TEST(suit_string_conversion) {
    ASSERT_EQ(SPADES, string_to_suit("Spades"));
    ASSERT_EQ(HEARTS, string_to_suit("Hearts"));
    ASSERT_EQ(CLUBS, string_to_suit("Clubs"));
    ASSERT_EQ(DIAMONDS, string_to_suit("Diamonds"));
}

TEST(rank_stream_operators) {
    ostringstream oss;
    oss << QUEEN;
    ASSERT_EQ("Queen", oss.str());
    
    istringstream iss("King");
    Rank r;
    iss >> r;
    ASSERT_EQ(KING, r);
}

TEST(suit_stream_operators) {
    ostringstream oss;
    oss << DIAMONDS;
    ASSERT_EQ("Diamonds", oss.str());
    
    istringstream iss("Clubs");
    Suit s;
    iss >> s;
    ASSERT_EQ(CLUBS, s);
}

// Run all tests
void run_card_tests() {
    cout << "=== Card Tests ===" << endl;
    
    run_test_card_default_constructor();
    run_test_card_parameterized_constructor();
    run_test_card_is_face_or_ace();
    run_test_card_equality_operators();
    run_test_card_comparison_operators();
    run_test_card_stream_output();
    run_test_card_stream_input();
    run_test_rank_string_conversion();
    run_test_suit_string_conversion();
    run_test_rank_stream_operators();
    run_test_suit_stream_operators();
    
    cout << "Card tests: " << tests_passed << "/" << tests_run << " passed" << endl;
}