#include "../Joker.hpp"
#include <iostream>
#include <memory>
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

// Joker Tests
TEST(basic_joker_properties) {
    BasicJoker joker;
    ASSERT_EQ("Joker", joker.get_name());
    ASSERT_EQ("+4 Mult", joker.get_description());
    ASSERT_EQ(COMMON, joker.get_rarity());
    ASSERT_EQ(2, joker.get_cost());
}

TEST(basic_joker_effect) {
    BasicJoker joker;
    HandInfo hand;
    hand.type = HIGH_CARD;
    hand.cards = {Card(ACE, SPADES)};
    
    int chips = 10;
    int mult = 1;
    
    joker.apply_effect(hand, chips, mult);
    
    ASSERT_EQ(10, chips); // Should not change chips
    ASSERT_EQ(5, mult);   // Should add +4 mult
}

TEST(pair_joker_properties) {
    PairJoker joker;
    ASSERT_EQ("Pair Joker", joker.get_name());
    ASSERT_EQ("+2 Mult per pair", joker.get_description());
    ASSERT_EQ(UNCOMMON, joker.get_rarity());
    ASSERT_EQ(5, joker.get_cost());
}

TEST(pair_joker_effect_with_pair) {
    PairJoker joker;
    HandInfo hand;
    hand.type = PAIR;
    hand.cards = {Card(ACE, SPADES), Card(ACE, HEARTS)};
    
    int chips = 10;
    int mult = 2;
    
    joker.apply_effect(hand, chips, mult);
    
    ASSERT_EQ(10, chips); // Should not change chips
    ASSERT_EQ(4, mult);   // Should add +2 mult for pair
}

TEST(pair_joker_effect_with_two_pair) {
    PairJoker joker;
    HandInfo hand;
    hand.type = TWO_PAIR;
    hand.cards = {Card(ACE, SPADES), Card(ACE, HEARTS), 
                  Card(KING, CLUBS), Card(KING, DIAMONDS), Card(QUEEN, SPADES)};
    
    int chips = 20;
    int mult = 2;
    
    joker.apply_effect(hand, chips, mult);
    
    ASSERT_EQ(20, chips); // Should not change chips
    ASSERT_EQ(6, mult);   // Should add +4 mult for two pairs
}

TEST(pair_joker_effect_no_pair) {
    PairJoker joker;
    HandInfo hand;
    hand.type = HIGH_CARD;
    hand.cards = {Card(ACE, SPADES), Card(KING, HEARTS)};
    
    int chips = 5;
    int mult = 1;
    
    joker.apply_effect(hand, chips, mult);
    
    ASSERT_EQ(5, chips); // Should not change
    ASSERT_EQ(1, mult);  // Should not change
}

TEST(flush_joker_properties) {
    FlushJoker joker;
    ASSERT_EQ("Flush Joker", joker.get_name());
    ASSERT_EQ("+3 Mult per card if poker hand is a Flush", joker.get_description());
    ASSERT_EQ(UNCOMMON, joker.get_rarity());
    ASSERT_EQ(6, joker.get_cost());
}

TEST(flush_joker_effect_with_flush) {
    FlushJoker joker;
    HandInfo hand;
    hand.type = FLUSH;
    hand.cards = {Card(TWO, SPADES), Card(FOUR, SPADES), Card(SIX, SPADES),
                  Card(EIGHT, SPADES), Card(TEN, SPADES)};
    
    int chips = 35;
    int mult = 4;
    
    joker.apply_effect(hand, chips, mult);
    
    ASSERT_EQ(35, chips);  // Should not change chips
    ASSERT_EQ(19, mult);   // Should add +15 mult (3 * 5 cards)
}

TEST(face_card_joker_properties) {
    FaceCardJoker joker;
    ASSERT_EQ("Face Card Joker", joker.get_name());
    ASSERT_EQ("+2 Mult per face card held", joker.get_description());
    ASSERT_EQ(COMMON, joker.get_rarity());
    ASSERT_EQ(4, joker.get_cost());
}

TEST(face_card_joker_effect) {
    FaceCardJoker joker;
    HandInfo hand;
    hand.type = HIGH_CARD;
    hand.cards = {Card(ACE, SPADES), Card(KING, HEARTS), Card(QUEEN, CLUBS),
                  Card(JACK, DIAMONDS), Card(TWO, SPADES)};
    
    int chips = 5;
    int mult = 1;
    
    joker.apply_effect(hand, chips, mult);
    
    ASSERT_EQ(5, chips);  // Should not change chips
    ASSERT_EQ(9, mult);   // Should add +8 mult (2 * 4 face cards)
}

TEST(joker_factory) {
    unique_ptr<Joker> basic_joker(Joker_factory("Joker"));
    ASSERT_EQ("Joker", basic_joker->get_name());
    
    unique_ptr<Joker> pair_joker(Joker_factory("Pair Joker"));
    ASSERT_EQ("Pair Joker", pair_joker->get_name());
    
    unique_ptr<Joker> flush_joker(Joker_factory("Flush Joker"));
    ASSERT_EQ("Flush Joker", flush_joker->get_name());
    
    unique_ptr<Joker> face_joker(Joker_factory("Face Card Joker"));
    ASSERT_EQ("Face Card Joker", face_joker->get_name());
}

TEST(joker_rarity_stream_output) {
    ostringstream oss;
    oss << COMMON;
    ASSERT_EQ("Common", oss.str());
    
    oss.str("");
    oss << UNCOMMON;
    ASSERT_EQ("Uncommon", oss.str());
    
    oss.str("");
    oss << RARE;
    ASSERT_EQ("Rare", oss.str());
    
    oss.str("");
    oss << LEGENDARY;
    ASSERT_EQ("Legendary", oss.str());
}

// Run all joker tests
void run_joker_tests() {
    cout << "=== Joker Tests ===" << endl;
    
    run_test_basic_joker_properties();
    run_test_basic_joker_effect();
    run_test_pair_joker_properties();
    run_test_pair_joker_effect_with_pair();
    run_test_pair_joker_effect_with_two_pair();
    run_test_pair_joker_effect_no_pair();
    run_test_flush_joker_properties();
    run_test_flush_joker_effect_with_flush();
    run_test_face_card_joker_properties();
    run_test_face_card_joker_effect();
    run_test_joker_factory();
    run_test_joker_rarity_stream_output();
    
    cout << "Joker tests completed" << endl;
}