#include "GameManager.hpp"
#include "Poker.hpp"
#include "Utils.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

// Analyze deck shuffling and hand distribution
void analyze_deck_shuffling() {
    cout << "Deck Shuffling Analysis" << endl;
    cout << "======================" << endl;
    
    map<string, int> hand_type_counts;
    const int num_tests = 1000;
    
    for (int test = 0; test < num_tests; ++test) {
        GameManager game;
        game.start_new_game();
        
        // Get the dealt hand
        const auto& hand = game.get_hand();
        
        // Evaluate the hand
        PokerHand poker_hand;
        for (const Card& card : hand) {
            poker_hand.add_card(card);
        }
        
        HandType type = poker_hand.evaluate_hand();
        string type_name = hand_type_to_string(type);
        hand_type_counts[type_name]++;
    }
    
    cout << "Hand Type Distribution (out of " << num_tests << " hands):" << endl;
    cout << "=================================================" << endl;
    
    for (const auto& pair : hand_type_counts) {
        double percentage = (double)pair.second / num_tests * 100.0;
        cout << left << setw(20) << pair.first << ": " 
             << setw(4) << pair.second << " (" 
             << fixed << setprecision(1) << percentage << "%)" << endl;
    }
    
    cout << endl;
    
    // Expected probabilities for 8-card hands (approximate)
    cout << "Expected vs Actual Analysis:" << endl;
    cout << "============================" << endl;
    
    map<string, double> expected_probabilities = {
        {"High Card", 50.0},    // Most common
        {"Pair", 35.0},         // Common
        {"Two Pair", 10.0},     // Less common
        {"Three of a Kind", 3.0}, // Rare
        {"Straight", 1.0},      // Very rare
        {"Flush", 0.8},         // Very rare
        {"Full House", 0.15},   // Extremely rare
        {"Four of a Kind", 0.05} // Extremely rare
    };
    
    for (const auto& expected : expected_probabilities) {
        string hand_type = expected.first;
        double expected_pct = expected.second;
        
        int actual_count = hand_type_counts[hand_type];
        double actual_pct = (double)actual_count / num_tests * 100.0;
        
        cout << left << setw(20) << hand_type << ": "
             << "Expected ~" << setw(5) << fixed << setprecision(1) << expected_pct << "%, "
             << "Actual " << setw(5) << actual_pct << "% ";
        
        if (abs(actual_pct - expected_pct) > expected_pct * 0.5) {
            cout << " ⚠️  DEVIATION";
        } else {
            cout << " ✓";
        }
        cout << endl;
    }
}

// Test individual deck shuffling quality
void test_shuffle_quality() {
    cout << "\nShuffle Quality Test" << endl;
    cout << "===================" << endl;
    
    const int num_shuffles = 100;
    map<string, int> first_card_counts;
    
    for (int i = 0; i < num_shuffles; ++i) {
        GameManager game;
        game.start_new_game();
        
        const auto& hand = game.get_hand();
        if (!hand.empty()) {
            stringstream ss;
            ss << hand[0]; // First card dealt
            first_card_counts[ss.str()]++;
        }
    }
    
    cout << "First card distribution (should be roughly even):" << endl;
    
    // Show top 10 most common first cards
    vector<pair<string, int>> sorted_cards(first_card_counts.begin(), first_card_counts.end());
    sort(sorted_cards.begin(), sorted_cards.end(), 
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    for (size_t i = 0; i < min(size_t(10), sorted_cards.size()); ++i) {
        double percentage = (double)sorted_cards[i].second / num_shuffles * 100.0;
        cout << sorted_cards[i].first << ": " << sorted_cards[i].second 
             << " (" << fixed << setprecision(1) << percentage << "%)" << endl;
    }
    
    // Check if any card appears too frequently
    double expected_frequency = 100.0 / 52.0; // ~1.92%
    bool bias_detected = false;
    
    for (const auto& card : sorted_cards) {
        double actual_frequency = (double)card.second / num_shuffles * 100.0;
        if (actual_frequency > expected_frequency * 2.0) {
            cout << "⚠️  Potential bias detected: " << card.first 
                 << " appears " << actual_frequency << "% (expected ~" 
                 << expected_frequency << "%)" << endl;
            bias_detected = true;
        }
    }
    
    if (!bias_detected) {
        cout << "✓ No obvious shuffle bias detected" << endl;
    }
}

// Test rank distribution in dealt hands
void test_rank_distribution() {
    cout << "\nRank Distribution Test" << endl;
    cout << "======================" << endl;
    
    const int num_tests = 500;
    map<Rank, int> rank_counts;
    
    for (int test = 0; test < num_tests; ++test) {
        GameManager game;
        game.start_new_game();
        
        const auto& hand = game.get_hand();
        for (const Card& card : hand) {
            rank_counts[card.get_rank()]++;
        }
    }
    
    cout << "Rank distribution in dealt hands:" << endl;
    
    for (int rank = TWO; rank <= ACE; ++rank) {
        Rank r = static_cast<Rank>(rank);
        int count = rank_counts[r];
        double percentage = (double)count / (num_tests * 8) * 100.0; // 8 cards per hand
        
        cout << left << setw(8);
        switch(r) {
            case TWO: cout << "Two"; break;
            case THREE: cout << "Three"; break;
            case FOUR: cout << "Four"; break;
            case FIVE: cout << "Five"; break;
            case SIX: cout << "Six"; break;
            case SEVEN: cout << "Seven"; break;
            case EIGHT: cout << "Eight"; break;
            case NINE: cout << "Nine"; break;
            case TEN: cout << "Ten"; break;
            case JACK: cout << "Jack"; break;
            case QUEEN: cout << "Queen"; break;
            case KING: cout << "King"; break;
            case ACE: cout << "Ace"; break;
        }
        
        cout << ": " << setw(4) << count << " (" 
             << fixed << setprecision(1) << percentage << "%)" << endl;
    }
    
    // Expected: each rank should appear ~7.69% of the time (100/13)
    double expected_pct = 100.0 / 13.0;
    cout << "\nExpected percentage per rank: ~" << fixed << setprecision(1) 
         << expected_pct << "%" << endl;
}

int main() {
    seed_random();
    
    cout << "Balatro C++ Deck Analysis Tool" << endl;
    cout << "==============================" << endl << endl;
    
    analyze_deck_shuffling();
    test_shuffle_quality();
    test_rank_distribution();
    
    cout << "\nAnalysis complete!" << endl;
    cout << "If you see significant deviations or bias warnings, " << endl;
    cout << "the shuffling algorithm may need improvement." << endl;
    
    return 0;
}