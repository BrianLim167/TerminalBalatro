#include "../Poker.hpp"
#include <iostream>
#include <vector>
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

// Poker Hand Tests
TEST(poker_hand_default_constructor) {
    PokerHand hand;
    ASSERT_EQ(HIGH_CARD, hand.get_type());
    ASSERT_EQ(0u, hand.size());
}

TEST(poker_hand_high_card) {
    vector<Card> cards = {
        Card(ACE, SPADES),
        Card(KING, HEARTS),
        Card(QUEEN, CLUBS),
        Card(JACK, DIAMONDS),
        Card(NINE, SPADES)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(HIGH_CARD, hand.get_type());
    ASSERT_EQ(5, hand.get_base_chips());
    ASSERT_EQ(1, hand.get_base_mult());
}

TEST(poker_hand_PAIR) {
    vector<Card> cards = {
        Card(ACE, SPADES),
        Card(ACE, HEARTS)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(PAIR, hand.get_type());
    ASSERT_EQ(10, hand.get_base_chips());
    ASSERT_EQ(2, hand.get_base_mult());
}

TEST(poker_hand_two_PAIR) {
    vector<Card> cards = {
        Card(ACE, SPADES),
        Card(ACE, HEARTS),
        Card(KING, CLUBS),
        Card(KING, DIAMONDS),
        Card(QUEEN, SPADES)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(TWO_PAIR, hand.get_type());
    ASSERT_EQ(20, hand.get_base_chips());
    ASSERT_EQ(2, hand.get_base_mult());
}

TEST(poker_hand_three_of_a_kind) {
    vector<Card> cards = {
        Card(ACE, SPADES),
        Card(ACE, HEARTS),
        Card(ACE, CLUBS)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(THREE_OF_KIND, hand.get_type());
    ASSERT_EQ(30, hand.get_base_chips());
    ASSERT_EQ(3, hand.get_base_mult());
}

TEST(poker_hand_STRAIGHT) {
    vector<Card> cards = {
        Card(TEN, SPADES),
        Card(JACK, HEARTS),
        Card(QUEEN, CLUBS),
        Card(KING, DIAMONDS),
        Card(ACE, SPADES)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(STRAIGHT, hand.get_type());
    ASSERT_EQ(30, hand.get_base_chips());
    ASSERT_EQ(4, hand.get_base_mult());
}

TEST(poker_hand_FLUSH) {
    vector<Card> cards = {
        Card(TWO, SPADES),
        Card(FOUR, SPADES),
        Card(SIX, SPADES),
        Card(EIGHT, SPADES),
        Card(TEN, SPADES)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(FLUSH, hand.get_type());
    ASSERT_EQ(35, hand.get_base_chips());
    ASSERT_EQ(4, hand.get_base_mult());
}

TEST(poker_hand_full_house) {
    vector<Card> cards = {
        Card(ACE, SPADES),
        Card(ACE, HEARTS),
        Card(ACE, CLUBS),
        Card(KING, DIAMONDS),
        Card(KING, SPADES)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(FULL_HOUSE, hand.get_type());
    ASSERT_EQ(40, hand.get_base_chips());
    ASSERT_EQ(4, hand.get_base_mult());
}

TEST(poker_hand_four_of_a_kind) {
    vector<Card> cards = {
        Card(ACE, SPADES),
        Card(ACE, HEARTS),
        Card(ACE, CLUBS),
        Card(ACE, DIAMONDS),
        Card(KING, SPADES)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(FOUR_OF_KIND, hand.get_type());
    ASSERT_EQ(60, hand.get_base_chips());
    ASSERT_EQ(7, hand.get_base_mult());
}

TEST(poker_hand_STRAIGHT_FLUSH) {
    vector<Card> cards = {
        Card(TEN, SPADES),
        Card(JACK, SPADES),
        Card(QUEEN, SPADES),
        Card(KING, SPADES),
        Card(ACE, SPADES)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(ROYAL_FLUSH, hand.get_type()); // This should be royal FLUSH actually
    ASSERT_EQ(100, hand.get_base_chips());
    ASSERT_EQ(8, hand.get_base_mult());
}

TEST(poker_hand_five_of_a_kind) {
    vector<Card> cards = {
        Card(ACE, SPADES),
        Card(ACE, HEARTS),
        Card(ACE, CLUBS),
        Card(ACE, DIAMONDS),
        Card(ACE, SPADES) // Duplicate for testing (normally impossible)
    };
    
    PokerHand hand(cards);
    ASSERT_EQ(FIVE_OF_KIND, hand.get_type());
    ASSERT_EQ(120, hand.get_base_chips());
    ASSERT_EQ(12, hand.get_base_mult());
}

TEST(poker_hand_set_cards) {
    PokerHand hand;
    vector<Card> cards = {Card(KING, HEARTS), Card(KING, SPADES)};
    
    hand.set_cards(cards);
    ASSERT_EQ(PAIR, hand.get_type());
    ASSERT_EQ(2u, hand.size());
}

TEST(poker_hand_add_card) {
    PokerHand hand;
    hand.add_card(Card(ACE, SPADES));
    hand.add_card(Card(ACE, HEARTS));
    
    ASSERT_EQ(PAIR, hand.get_type());
    ASSERT_EQ(2u, hand.size());
}

TEST(poker_hand_clear) {
    PokerHand hand;
    hand.add_card(Card(ACE, SPADES));
    hand.clear();
    
    ASSERT_EQ(0u, hand.size());
    ASSERT_EQ(HIGH_CARD, hand.get_type());
}

TEST(hand_type_to_string) {
    ASSERT_EQ("High Card", hand_type_to_string(HIGH_CARD));
    ASSERT_EQ("Pair", hand_type_to_string(PAIR));
    ASSERT_EQ("Royal Flush", hand_type_to_string(ROYAL_FLUSH));
    ASSERT_EQ("Five of a Kind", hand_type_to_string(FIVE_OF_KIND));
}

// Run all poker tests
void run_poker_tests() {
    cout << "=== Poker Tests ===" << endl;
    
    run_test_poker_hand_default_constructor();
    run_test_poker_hand_high_card();
    run_test_poker_hand_PAIR();
    run_test_poker_hand_two_PAIR();
    run_test_poker_hand_three_of_a_kind();
    run_test_poker_hand_STRAIGHT();
    run_test_poker_hand_FLUSH();
    run_test_poker_hand_full_house();
    run_test_poker_hand_four_of_a_kind();
    run_test_poker_hand_STRAIGHT_FLUSH();
    run_test_poker_hand_five_of_a_kind();
    run_test_poker_hand_set_cards();
    run_test_poker_hand_add_card();
    run_test_poker_hand_clear();
    run_test_hand_type_to_string();
    
    cout << "Poker tests completed" << endl;
}