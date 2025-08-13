#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <string>
#include <map>
#include <vector>

struct GameStats {
    int games_played = 0;
    int games_won = 0;
    int total_score = 0;
    int highest_ante = 0;
    int total_hands_played = 0;
    int total_money_earned = 0;
    int jokers_bought = 0;
    std::map<std::string, int> hand_types_played; // Track frequency of each hand type
    std::map<std::string, int> jokers_used; // Track frequency of each joker type
};

class Statistics {
public:
    //EFFECTS Initializes statistics with default values
    Statistics();
    
    //EFFECTS Records a completed game
    void record_game(bool won, int final_ante, int final_score, int money_earned);
    
    //EFFECTS Records a hand being played
    void record_hand_played(const std::string& hand_type, int score);
    
    //EFFECTS Records a joker being purchased
    void record_joker_bought(const std::string& joker_name, int cost);
    
    //EFFECTS Returns current game statistics
    const GameStats& get_stats() const;
    
    //EFFECTS Displays formatted statistics
    void display_statistics() const;
    
    //EFFECTS Saves statistics to file
    bool save_statistics(const std::string& filename) const;
    
    //EFFECTS Loads statistics from file
    bool load_statistics(const std::string& filename);
    
    //EFFECTS Resets all statistics to zero
    void reset_statistics();

private:
    GameStats stats;
};

#endif // STATISTICS_HPP