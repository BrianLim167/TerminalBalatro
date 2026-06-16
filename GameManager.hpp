#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <vector>
#include <memory>
#include <string>
#include "Card.hpp"
#include "Poker.hpp"
#include "Joker.hpp"

enum BlindType {
    SMALL_BLIND = 0,
    BIG_BLIND = 1,
    BOSS_BLIND = 2
};

enum GameState {
    PLAYING,
    SHOP,
    GAME_OVER,
    VICTORY
};

class GameManager {
public:
    //EFFECTS Initializes a new Balatro game
    GameManager();
    
    //EFFECTS Destructor cleans up jokers
    ~GameManager();
    
    //EFFECTS Starts a new game, initializes deck and game state
    void start_new_game();
    
    //EFFECTS Deals 8 cards from deck to player's hand
    void deal_hand();
    
    //EFFECTS Plays the selected cards and calculates score
    //REQUIRES selected_indices contains valid hand indices
    //MODIFIES Updates score, hands_left
    int play_hand(const std::vector<int>& selected_indices);
    
    //EFFECTS Discards selected cards and draws new ones
    //REQUIRES selected_indices contains valid hand indices and discards_left > 0
    //MODIFIES Updates hand, discards_left
    void discard_cards(const std::vector<int>& selected_indices);
    
    //EFFECTS Returns true if current blind is completed (score >= chips_needed)
    bool is_blind_complete() const;
    
    //EFFECTS Advances to next blind or ante, returns true if game continues
    //MODIFIES Updates ante, current_blind, resets counters
    bool advance_blind();
    
    //EFFECTS Enters shop phase for buying jokers/cards
    void enter_shop();
    
    //EFFECTS Buys a joker from the shop
    //REQUIRES joker_name is valid and player has enough money
    //MODIFIES Updates money, adds joker to collection
    bool buy_joker(const std::string& joker_name);
    
    //EFFECTS Spends money if player has enough
    //REQUIRES cost >= 0
    //MODIFIES Updates money
    bool spend_money(int cost);
    
    //EFFECTS Sells a joker from player's collection
    //REQUIRES joker_index is valid
    //MODIFIES Updates money, removes joker from collection
    void sell_joker(int joker_index);
    
    //EFFECTS Returns current game state
    GameState get_game_state() const;
    
    //EFFECTS Returns current ante (1-8)
    int get_ante() const;
    
    //EFFECTS Returns current blind type
    BlindType get_current_blind() const;
    
    //EFFECTS Returns chips needed to beat current blind
    int get_chips_needed() const;
    
    //EFFECTS Returns current score this blind
    int get_current_score() const;
    
    //EFFECTS Returns hands remaining this blind
    int get_hands_left() const;
    
    //EFFECTS Returns discards remaining this blind
    int get_discards_left() const;
    
    //EFFECTS Returns player's current money
    int get_money() const;
    
    //EFFECTS Returns player's current hand
    const std::vector<Card>& get_hand() const;
    
    //EFFECTS Returns player's jokers
    const std::vector<std::unique_ptr<Joker>>& get_jokers() const;
    
    //EFFECTS Returns string representation of current blind
    std::string get_blind_name() const;
    
    //EFFECTS Adds a joker by name, bypassing shop/cost
    //REQUIRES name is a valid joker name
    void add_joker_cheat(const std::string& joker_name);
    
    //EFFECTS Adds the specified amount of money
    void add_money_cheat(int amount);
    
    //EFFECTS Adds to current score
    void add_score_cheat(int amount);
    
    //EFFECTS Sets hands remaining
    void set_hands_cheat(int count);
    
    //EFFECTS Sets discards remaining
    void set_discards_cheat(int count);
    
    //EFFECTS Completes the current blind immediately
    void win_blind_cheat();
    
    //EFFECTS Saves current game state to file
    //REQUIRES filename is a valid path
    //MODIFIES Creates/overwrites save file
    bool save_game(const std::string& filename) const;
    
    //EFFECTS Loads game state from file
    //REQUIRES filename exists and contains valid save data
    //MODIFIES Restores all game state from save file
    bool load_game(const std::string& filename);

private:
    // Game state
    int ante;
    BlindType current_blind;
    GameState game_state;
    
    // Scoring
    int chips_needed;
    int current_score;
    int hands_left;
    int discards_left;
    int money;
    
    // Cards and deck
    std::vector<Card> deck;
    std::vector<Card> hand;
    std::vector<std::unique_ptr<Joker>> jokers;
    
    // Helper methods
    //EFFECTS Creates and shuffles a standard 52-card deck
    void initialize_deck();
    
    //EFFECTS Shuffles the current deck
    void shuffle_deck();
    
    //EFFECTS Calculates chips needed for current blind
    int calculate_blind_requirement() const;
    
    //EFFECTS Applies all joker effects to the given score
    //MODIFIES chips and mult based on joker effects
    void apply_joker_effects(const HandInfo& hand_info, int& chips, int& mult);
    
};

//EFFECTS Prints BlindType to stream
std::ostream& operator<<(std::ostream& os, BlindType blind);

//EFFECTS Prints GameState to stream
std::ostream& operator<<(std::ostream& os, GameState state);

#endif // GAMEMANAGER_HPP