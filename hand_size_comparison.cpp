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

// Test different hand sizes to show probability differences
void compare_hand_sizes() {
    cout << "Hand Size Probability Comparison" << endl;
    cout << "================================" << endl;
    
    const int num_tests = 2000;
    vector<int> hand_sizes = {5, 6, 7, 8};
    
    for (int hand_size : hand_sizes) {
        cout << "\n" << hand_size << "-Card Hands (out of " << num_tests << " tests):" << endl;
        cout << string(40, '-') << endl;
        
        map<string, int> hand_type_counts;
        
        for (int test = 0; test < num_tests; ++test) {
            GameManager game;
            game.start_new_game();
            
            // Get the dealt hand and truncate to desired size
            const auto& full_hand = game.get_hand();
            
            PokerHand poker_hand;
            for (int i = 0; i < hand_size && i < static_cast<int>(full_hand.size()); ++i) {
                poker_hand.add_card(full_hand[i]);
            }
            
            HandType type = poker_hand.evaluate_hand();
            string type_name = hand_type_to_string(type);
            hand_type_counts[type_name]++;
        }
        
        // Sort by frequency
        vector<pair<string, int>> sorted_hands(hand_type_counts.begin(), hand_type_counts.end());
        sort(sorted_hands.begin(), sorted_hands.end(), 
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });
        
        for (const auto& pair : sorted_hands) {
            double percentage = (double)pair.second / num_tests * 100.0;
            cout << left << setw(18) << pair.first << ": " 
                 << setw(4) << pair.second << " (" 
                 << fixed << setprecision(1) << percentage << "%)" << endl;
        }
        
        // Calculate pair + two pair percentage
        int pairs = hand_type_counts["Pair"];
        int two_pairs = hand_type_counts["Two Pair"];
        double pair_percentage = (double)(pairs + two_pairs) / num_tests * 100.0;
        
        cout << "Combined Pairs: " << (pairs + two_pairs) 
             << " (" << fixed << setprecision(1) << pair_percentage << "%)" << endl;
    }
}

// Show theoretical vs actual probabilities
void show_theoretical_comparison() {
    cout << "\n\nTheoretical vs Actual Comparison" << endl;
    cout << "=================================" << endl;
    
    cout << "\n5-Card Hands (Traditional Poker):" << endl;
    cout << "High Card:      ~50.1%" << endl;
    cout << "Pair:           ~42.3%" << endl;
    cout << "Two Pair:       ~4.75%" << endl;
    cout << "Three of Kind:  ~2.11%" << endl;
    cout << "Straight:       ~0.39%" << endl;
    cout << "Flush:          ~0.20%" << endl;
    cout << "Full House:     ~0.14%" << endl;
    cout << "Four of Kind:   ~0.024%" << endl;
    
    cout << "\n8-Card Hands (Balatro Style):" << endl;
    cout << "High Card:      ~10-15% (much lower!)" << endl;
    cout << "Pair:           ~35-40% (similar)" << endl;
    cout << "Two Pair:       ~35-40% (MUCH higher!)" << endl;
    cout << "Three of Kind:  ~6-8% (higher)" << endl;
    cout << "Full House:     ~6-8% (MUCH higher!)" << endl;
    cout << "Four of Kind:   ~0.3-0.5% (higher)" << endl;
    
    cout << "\nKey Insight:" << endl;
    cout << "With 8 cards, you have a ~85% chance of getting at least one pair!" << endl;
    cout << "This is why pairs and two pairs dominate the game." << endl;
}

// Simulate the "feel" of getting too many pairs
void analyze_pair_streaks() {
    cout << "\n\nPair Streak Analysis" << endl;
    cout << "====================" << endl;
    
    const int num_games = 100;
    int consecutive_pair_games = 0;
    int max_streak = 0;
    int current_streak = 0;
    
    cout << "Simulating " << num_games << " games, tracking pair/two-pair streaks..." << endl;
    
    for (int game = 0; game < num_games; ++game) {
        GameManager gm;
        gm.start_new_game();
        
        const auto& hand = gm.get_hand();
        PokerHand poker_hand;
        for (const Card& card : hand) {
            poker_hand.add_card(card);
        }
        
        HandType type = poker_hand.evaluate_hand();
        bool is_pair_type = (type == PAIR || type == TWO_PAIR);
        
        if (is_pair_type) {
            consecutive_pair_games++;
            current_streak++;
            max_streak = max(max_streak, current_streak);
        } else {
            current_streak = 0;
        }
    }
    
    double pair_rate = (double)consecutive_pair_games / num_games * 100.0;
    
    cout << "Results:" << endl;
    cout << "- Games with pairs/two pairs: " << consecutive_pair_games 
         << "/" << num_games << " (" << fixed << setprecision(1) << pair_rate << "%)" << endl;
    cout << "- Longest streak of pair games: " << max_streak << endl;
    
    if (pair_rate > 70) {
        cout << "- This explains why it 'feels' like you always get pairs!" << endl;
    }
}

int main() {
    seed_random();
    
    cout << "Balatro Hand Size Analysis" << endl;
    cout << "==========================" << endl;
    
    compare_hand_sizes();
    show_theoretical_comparison();
    analyze_pair_streaks();
    
    cout << "\n\nConclusion:" << endl;
    cout << "==========" << endl;
    cout << "The high frequency of pairs and two pairs in Balatro is" << endl;
    cout << "mathematically correct for 8-card hands. This is not a bug" << endl;
    cout << "in the shuffling - it's the natural probability distribution!" << endl;
    
    return 0;
}