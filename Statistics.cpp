#include "Statistics.hpp"
#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

//EFFECTS Initializes statistics with default values
Statistics::Statistics() {
    // stats already initialized with default values in struct
}

//EFFECTS Records a completed game
void Statistics::record_game(bool won, int final_ante, int final_score, int money_earned) {
    stats.games_played++;
    if (won) {
        stats.games_won++;
    }
    stats.total_score += final_score;
    stats.highest_ante = max(stats.highest_ante, final_ante);
    stats.total_money_earned += money_earned;
}

//EFFECTS Records a hand being played
void Statistics::record_hand_played(const string& hand_type, int score) {
    stats.total_hands_played++;
    stats.total_score += score; // Track total score across all hands
    stats.hand_types_played[hand_type]++;
}

//EFFECTS Records a joker being purchased
void Statistics::record_joker_bought(const string& joker_name, int cost) {
    stats.jokers_bought++;
    stats.jokers_used[joker_name]++;
    stats.total_money_earned -= cost; // Subtract cost from total earnings
}

//EFFECTS Returns current game statistics
const GameStats& Statistics::get_stats() const {
    return stats;
}

//EFFECTS Displays formatted statistics
void Statistics::display_statistics() const {
    clear_screen();
    print_centered("GAME STATISTICS", 60);
    print_separator();
    cout << endl;
    
    // Basic stats
    cout << "Games Played: " << stats.games_played << endl;
    cout << "Games Won: " << stats.games_won << endl;
    if (stats.games_played > 0) {
        double win_rate = (double)stats.games_won / stats.games_played * 100.0;
        cout << "Win Rate: " << fixed << setprecision(1) << win_rate << "%" << endl;
    }
    cout << "Highest Ante Reached: " << stats.highest_ante << endl;
    cout << "Total Score: " << format_number(stats.total_score) << endl;
    cout << "Total Hands Played: " << stats.total_hands_played << endl;
    cout << "Total Money Earned: $" << stats.total_money_earned << endl;
    cout << "Jokers Bought: " << stats.jokers_bought << endl;
    
    if (stats.games_played > 0) {
        cout << "Average Score per Game: " << format_number(stats.total_score / stats.games_played) << endl;
    }
    
    cout << endl;
    
    // Hand types played
    if (!stats.hand_types_played.empty()) {
        cout << "Most Played Hand Types:" << endl;
        
        // Convert map to vector of pairs for sorting
        vector<pair<string, int>> hand_pairs(stats.hand_types_played.begin(), stats.hand_types_played.end());
        sort(hand_pairs.begin(), hand_pairs.end(), 
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });
        
        for (size_t i = 0; i < min(size_t(5), hand_pairs.size()); ++i) {
            cout << "  " << hand_pairs[i].first << ": " << hand_pairs[i].second << endl;
        }
        cout << endl;
    }
    
    // Favorite jokers
    if (!stats.jokers_used.empty()) {
        cout << "Most Used Jokers:" << endl;
        
        // Convert map to vector of pairs for sorting
        vector<pair<string, int>> joker_pairs(stats.jokers_used.begin(), stats.jokers_used.end());
        sort(joker_pairs.begin(), joker_pairs.end(), 
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });
        
        for (size_t i = 0; i < min(size_t(5), joker_pairs.size()); ++i) {
            cout << "  " << joker_pairs[i].first << ": " << joker_pairs[i].second << endl;
        }
        cout << endl;
    }
    
    wait_for_enter();
}

//EFFECTS Saves statistics to file
bool Statistics::save_statistics(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "BALATRO_STATS_V1" << endl;
    file << stats.games_played << endl;
    file << stats.games_won << endl;
    file << stats.total_score << endl;
    file << stats.highest_ante << endl;
    file << stats.total_hands_played << endl;
    file << stats.total_money_earned << endl;
    file << stats.jokers_bought << endl;
    
    // Save hand types
    file << stats.hand_types_played.size() << endl;
    for (const auto& pair : stats.hand_types_played) {
        file << pair.first << " " << pair.second << endl;
    }
    
    // Save joker usage
    file << stats.jokers_used.size() << endl;
    for (const auto& pair : stats.jokers_used) {
        file << pair.first << " " << pair.second << endl;
    }
    
    return true;
}

//EFFECTS Loads statistics from file
bool Statistics::load_statistics(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    string version;
    getline(file, version);
    if (version != "BALATRO_STATS_V1") {
        return false;
    }
    
    file >> stats.games_played >> stats.games_won >> stats.total_score;
    file >> stats.highest_ante >> stats.total_hands_played;
    file >> stats.total_money_earned >> stats.jokers_bought;
    
    // Load hand types
    size_t hand_types_count;
    file >> hand_types_count;
    file.ignore(); // consume newline
    stats.hand_types_played.clear();
    for (size_t i = 0; i < hand_types_count; ++i) {
        string hand_type;
        int count;
        file >> hand_type >> count;
        stats.hand_types_played[hand_type] = count;
    }
    
    // Load joker usage
    size_t jokers_count;
    file >> jokers_count;
    stats.jokers_used.clear();
    for (size_t i = 0; i < jokers_count; ++i) {
        string joker_name;
        int count;
        file >> joker_name >> count;
        stats.jokers_used[joker_name] = count;
    }
    
    return true;
}

//EFFECTS Resets all statistics to zero
void Statistics::reset_statistics() {
    stats = GameStats(); // Reset to default values
}