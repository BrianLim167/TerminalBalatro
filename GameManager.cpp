#include "GameManager.hpp"
#include "ImprovedShuffle.hpp"
#include <algorithm>
#include <random>
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>

using namespace std;

//EFFECTS Initializes a new Balatro game
GameManager::GameManager() 
    : ante(1), current_blind(SMALL_BLIND), game_state(PLAYING),
      chips_needed(300), current_score(0), hands_left(4), discards_left(3), money(4) {
    initialize_deck();
}

//EFFECTS Destructor cleans up jokers
GameManager::~GameManager() {
    // unique_ptr automatically cleans up jokers
}

//EFFECTS Starts a new game, initializes deck and game state
void GameManager::start_new_game() {
    ante = 1;
    current_blind = SMALL_BLIND;
    game_state = PLAYING;
    current_score = 0;
    hands_left = 4;
    discards_left = 3;
    money = 4;
    
    jokers.clear();
    initialize_deck();
    deal_hand();
    chips_needed = calculate_blind_requirement();
}

//EFFECTS Deals 8 cards from deck to player's hand
void GameManager::deal_hand() {
    hand.clear();
    
    if (deck.size() < 8) {
        initialize_deck(); // Reshuffle if needed
    }
    
    for (int i = 0; i < 8 && !deck.empty(); ++i) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
}

//EFFECTS Plays the selected cards and calculates score
//REQUIRES selected_indices contains valid hand indices
//MODIFIES Updates score, hands_left
int GameManager::play_hand(const vector<int>& selected_indices) {
    assert(hands_left > 0);
    assert(selected_indices.size() >= 1 && selected_indices.size() <= 5);
    
    // Extract selected cards
    vector<Card> played_cards;
    for (int index : selected_indices) {
        assert(index >= 0 && index < static_cast<int>(hand.size()));
        played_cards.push_back(hand[index]);
    }
    
    // Evaluate hand using PokerHand class
    PokerHand poker_hand(played_cards);
    
    // Create HandInfo for joker effects
    HandInfo hand_info;
    hand_info.type = poker_hand.get_type();
    hand_info.cards = played_cards;
    hand_info.base_chips = poker_hand.get_base_chips();
    hand_info.base_mult = poker_hand.get_base_mult();
    
    // Get base scoring from poker hand
    int chips = poker_hand.get_base_chips();
    int mult = poker_hand.get_base_mult();
    
    // Add chip values for individual cards
    for (const Card& card : played_cards) {
        if (card.get_rank() >= TEN) {
            chips += 10; // Face cards worth 10 chips
        } else {
            chips += static_cast<int>(card.get_rank()) + 2; // Number cards worth face value
        }
    }
    
    // Apply joker effects
    apply_joker_effects(hand_info, chips, mult);
    
    // Calculate final score
    int hand_score = chips * mult;
    current_score += hand_score;
    hands_left--;
    
    // Remove played cards from hand and draw new ones
    vector<Card> new_hand;
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
        bool was_played = false;
        for (int played_index : selected_indices) {
            if (i == played_index) {
                was_played = true;
                break;
            }
        }
        if (!was_played) {
            new_hand.push_back(hand[i]);
        }
    }
    
    hand = new_hand;
    
    // Draw replacement cards
    while (hand.size() < 8 && !deck.empty()) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
    
    return hand_score;
}

//EFFECTS Discards selected cards and draws new ones
//REQUIRES selected_indices contains valid hand indices and discards_left > 0
//MODIFIES Updates hand, discards_left
void GameManager::discard_cards(const vector<int>& selected_indices) {
    assert(discards_left > 0);
    
    // Remove discarded cards
    vector<Card> new_hand;
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
        bool was_discarded = false;
        for (int discard_index : selected_indices) {
            if (i == discard_index) {
                was_discarded = true;
                break;
            }
        }
        if (!was_discarded) {
            new_hand.push_back(hand[i]);
        }
    }
    
    hand = new_hand;
    discards_left--;
    
    // Draw replacement cards
    while (hand.size() < 8 && !deck.empty()) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
}

//EFFECTS Returns true if current blind is completed (score >= chips_needed)
bool GameManager::is_blind_complete() const {
    return current_score >= chips_needed;
}

//EFFECTS Advances to next blind or ante, returns true if game continues
//MODIFIES Updates ante, current_blind, resets counters
bool GameManager::advance_blind() {
    // Award money for remaining hands
    money += hands_left;
    
    if (current_blind == SMALL_BLIND) {
        current_blind = BIG_BLIND;
    } else if (current_blind == BIG_BLIND) {
        current_blind = BOSS_BLIND;
    } else { // BOSS_BLIND
        ante++;
        current_blind = SMALL_BLIND;
        if (ante > 8) {
            game_state = VICTORY;
            return false;
        }
    }
    
    // Reset for next blind
    current_score = 0;
    hands_left = 4;
    discards_left = 3;
    chips_needed = calculate_blind_requirement();
    
    deal_hand();
    return true;
}

//EFFECTS Enters shop phase for buying jokers/cards
void GameManager::enter_shop() {
    game_state = SHOP;
}

//EFFECTS Buys a joker from the shop
//REQUIRES joker_name is valid and player has enough money
//MODIFIES Updates money, adds joker to collection
bool GameManager::buy_joker(const string& joker_name) {
    unique_ptr<Joker> joker(Joker_factory(joker_name));
    if (!joker || money < joker->get_cost()) {
        return false;
    }
    
    money -= joker->get_cost();
    jokers.push_back(std::move(joker));
    return true;
}

//EFFECTS Spends money if player has enough
//REQUIRES cost >= 0
//MODIFIES Updates money
bool GameManager::spend_money(int cost) {
    if (cost < 0 || money < cost) {
        return false;
    }
    
    money -= cost;
    return true;
}

//EFFECTS Sells a joker from player's collection
//REQUIRES joker_index is valid
//MODIFIES Updates money, removes joker from collection
void GameManager::sell_joker(int joker_index) {
    assert(joker_index >= 0 && joker_index < static_cast<int>(jokers.size()));
    
    money += jokers[joker_index]->get_cost() / 2; // Sell for half price
    jokers.erase(jokers.begin() + joker_index);
}

//EFFECTS Returns current game state
GameState GameManager::get_game_state() const {
    return game_state;
}

//EFFECTS Returns current ante (1-8)
int GameManager::get_ante() const {
    return ante;
}

//EFFECTS Returns current blind type
BlindType GameManager::get_current_blind() const {
    return current_blind;
}

//EFFECTS Returns chips needed to beat current blind
int GameManager::get_chips_needed() const {
    return chips_needed;
}

//EFFECTS Returns current score this blind
int GameManager::get_current_score() const {
    return current_score;
}

//EFFECTS Returns hands remaining this blind
int GameManager::get_hands_left() const {
    return hands_left;
}

//EFFECTS Returns discards remaining this blind
int GameManager::get_discards_left() const {
    return discards_left;
}

//EFFECTS Returns player's current money
int GameManager::get_money() const {
    return money;
}

//EFFECTS Returns player's current hand
const vector<Card>& GameManager::get_hand() const {
    return hand;
}

//EFFECTS Returns player's jokers
const vector<unique_ptr<Joker>>& GameManager::get_jokers() const {
    return jokers;
}

//EFFECTS Returns string representation of current blind
string GameManager::get_blind_name() const {
    switch (current_blind) {
        case SMALL_BLIND:
            return "Small Blind";
        case BIG_BLIND:
            return "Big Blind";
        case BOSS_BLIND:
            return "Boss Blind";
        default:
            return "Unknown";
    }
}

// Private helper methods

//EFFECTS Creates and shuffles a standard 52-card deck
void GameManager::initialize_deck() {
    deck = ImprovedShuffle::create_shuffled_deck();
}

//EFFECTS Shuffles the current deck
void GameManager::shuffle_deck() {
    ImprovedShuffle::shuffle_deck(deck);
}

//EFFECTS Calculates chips needed for current blind
int GameManager::calculate_blind_requirement() const {
    int base_requirement = 300;
    
    switch (current_blind) {
        case SMALL_BLIND:
            return base_requirement + (ante - 1) * 150;
        case BIG_BLIND:
            return (base_requirement + (ante - 1) * 150) * 2;
        case BOSS_BLIND:
            return (base_requirement + (ante - 1) * 150) * 3;
        default:
            return base_requirement;
    }
}

//EFFECTS Applies all joker effects to the given score
//MODIFIES chips and mult based on joker effects
void GameManager::apply_joker_effects(const HandInfo& hand_info, int& chips, int& mult) {
    for (const auto& joker : jokers) {
        joker->apply_effect(hand_info, chips, mult);
    }
}


// Cheat methods

void GameManager::add_joker_cheat(const string& joker_name) {
    unique_ptr<Joker> joker(Joker_factory(joker_name));
    if (joker) {
        jokers.push_back(std::move(joker));
    }
}

void GameManager::add_money_cheat(int amount) {
    money += amount;
}

void GameManager::add_score_cheat(int amount) {
    current_score += amount;
}

void GameManager::set_hands_cheat(int count) {
    hands_left = count;
}

void GameManager::set_discards_cheat(int count) {
    discards_left = count;
}

void GameManager::win_blind_cheat() {
    current_score = chips_needed;
}

//EFFECTS Prints BlindType to stream
ostream& operator<<(ostream& os, BlindType blind) {
    switch (blind) {
        case SMALL_BLIND:
            os << "Small Blind";
            break;
        case BIG_BLIND:
            os << "Big Blind";
            break;
        case BOSS_BLIND:
            os << "Boss Blind";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}

//EFFECTS Prints GameState to stream
ostream& operator<<(ostream& os, GameState state) {
    switch (state) {
        case PLAYING:
            os << "Playing";
            break;
        case SHOP:
            os << "Shop";
            break;
        case GAME_OVER:
            os << "Game Over";
            break;
        case VICTORY:
            os << "Victory";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}

//EFFECTS Saves current game state to file
//REQUIRES filename is a valid path
//MODIFIES Creates/overwrites save file
bool GameManager::save_game(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Save basic game state
    file << "BALATRO_SAVE_V1" << endl;
    file << ante << endl;
    file << static_cast<int>(current_blind) << endl;
    file << static_cast<int>(game_state) << endl;
    file << chips_needed << endl;
    file << current_score << endl;
    file << hands_left << endl;
    file << discards_left << endl;
    file << money << endl;
    
    // Save deck
    file << deck.size() << endl;
    for (const Card& card : deck) {
        file << card << endl;
    }
    
    // Save hand
    file << hand.size() << endl;
    for (const Card& card : hand) {
        file << card << endl;
    }
    
    // Save jokers (just the names for now)
    file << jokers.size() << endl;
    for (const auto& joker : jokers) {
        file << joker->get_name() << endl;
    }
    
    return true;
}

//EFFECTS Loads game state from file
//REQUIRES filename exists and contains valid save data
//MODIFIES Restores all game state from save file
bool GameManager::load_game(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    string version;
    getline(file, version);
    if (version != "BALATRO_SAVE_V1") {
        return false;
    }
    
    // Load basic game state
    int blind_type, game_state_int;
    file >> ante >> blind_type >> game_state_int;
    file >> chips_needed >> current_score >> hands_left >> discards_left >> money;
    
    current_blind = static_cast<BlindType>(blind_type);
    game_state = static_cast<GameState>(game_state_int);
    
    // Load deck
    size_t deck_size;
    file >> deck_size;
    deck.clear();
    file.ignore(); // consume newline
    for (size_t i = 0; i < deck_size; ++i) {
        Card card;
        file >> card;
        deck.push_back(card);
        file.ignore(); // consume newline
    }
    
    // Load hand
    size_t hand_size;
    file >> hand_size;
    hand.clear();
    for (size_t i = 0; i < hand_size; ++i) {
        Card card;
        file >> card;
        hand.push_back(card);
        file.ignore(); // consume newline
    }
    
    // Load jokers
    size_t jokers_size;
    file >> jokers_size;
    jokers.clear();
    file.ignore(); // consume newline
    for (size_t i = 0; i < jokers_size; ++i) {
        string joker_name;
        getline(file, joker_name);
        // Recreate joker by name (simplified approach)
        buy_joker(joker_name);  // This will create the joker and deduct money, but we'll restore money
    }
    
    // Fix money (buy_joker may have modified it)
    // Re-read the money value from the save file
    file.clear();
    file.seekg(0);
    string line;
    for (int i = 0; i < 8; ++i) {
        getline(file, line);
    }
    getline(file, line);
    money = stoi(line);
    
    return true;
}