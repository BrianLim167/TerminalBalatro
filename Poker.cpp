#include "Poker.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <cassert>

using namespace std;

//EFFECTS Initializes empty poker hand
PokerHand::PokerHand() : type(HIGH_CARD), base_chips(5), base_mult(1) {}

//EFFECTS Initializes poker hand with given cards and evaluates it
PokerHand::PokerHand(const vector<Card>& cards) : cards(cards) {
    type = evaluate_hand();
    calculate_base_values();
}

//EFFECTS Evaluates the poker hand and returns the hand type
HandType PokerHand::evaluate_hand() {
    if (cards.empty()) {
        type = HIGH_CARD;
        return type;
    }
    
    // Check Balatro special hands first (highest priority)
    if (is_flushFive()) {
        type = FLUSH_FIVE;
    } else if (is_flushHouse()) {
        type = FLUSH_HOUSE;
    } else if (is_fiveOfKind()) {
        type = FIVE_OF_KIND;
    }
    // Check standard poker hands
    else if (is_royalFlush()) {
        type = ROYAL_FLUSH;
    } else if (is_straightFlush()) {
        type = STRAIGHT_FLUSH;
    } else if (is_fourOfKind()) {
        type = FOUR_OF_KIND;
    } else if (is_fullHouse()) {
        type = FULL_HOUSE;
    } else if (is_flush()) {
        type = FLUSH;
    } else if (is_straight()) {
        type = STRAIGHT;
    } else if (is_threeOfKind()) {
        type = THREE_OF_KIND;
    } else if (is_twoPair()) {
        type = TWO_PAIR;
    } else if (is_pair()) {
        type = PAIR;
    } else {
        type = HIGH_CARD;
    }
    
    return type;
}

//EFFECTS Returns base chips for current hand type
int PokerHand::get_base_chips() const {
    return base_chips;
}

//EFFECTS Returns base multiplier for current hand type
int PokerHand::get_base_mult() const {
    return base_mult;
}

//EFFECTS Calculates basic score (chips * mult) without joker effects
int PokerHand::calculate_score() const {
    int total_chips = base_chips;
    
    // Add chip values for individual cards
    for (const Card& card : cards) {
        if (card.get_rank() >= TEN) {
            total_chips += 10; // Face cards worth 10 chips
        } else {
            total_chips += static_cast<int>(card.get_rank()) + 2; // Number cards worth face value
        }
    }
    
    return total_chips * base_mult;
}

//EFFECTS Returns current hand type
HandType PokerHand::get_type() const {
    return type;
}

//EFFECTS Sets new cards and re-evaluates hand
void PokerHand::set_cards(const vector<Card>& new_cards) {
    cards = new_cards;
    type = evaluate_hand();
    calculate_base_values();
}

//EFFECTS Adds a card to the hand
void PokerHand::add_card(const Card& card) {
    cards.push_back(card);
    type = evaluate_hand();
    calculate_base_values();
}

//EFFECTS Clears all cards from hand
void PokerHand::clear() {
    cards.clear();
    type = HIGH_CARD;
    calculate_base_values();
}

//EFFECTS Returns number of cards in hand
size_t PokerHand::size() const {
    return cards.size();
}

// Private helper methods

//EFFECTS Returns true if hand is a flush (all same suit)
bool PokerHand::is_flush() const {
    if (cards.size() < 5) return false;
    
    Suit first_suit = cards[0].get_suit();
    for (const Card& card : cards) {
        if (card.get_suit() != first_suit) {
            return false;
        }
    }
    return true;
}

//EFFECTS Returns true if hand is high card (no other patterns)
bool PokerHand::is_highCard() const {
    return !is_pair() && !is_straight() && !is_flush();
}

//EFFECTS Returns true if hand contains at least one pair
bool PokerHand::is_pair() const {
    vector<int> rank_counts = get_rank_counts();
    
    for (int count : rank_counts) {
        if (count >= 2) {
            return true;
        }
    }
    return false;
}

//EFFECTS Returns true if hand contains two pairs
bool PokerHand::is_twoPair() const {
    vector<int> rank_counts = get_rank_counts();
    
    int pairs = 0;
    for (int count : rank_counts) {
        if (count >= 2) {
            pairs++;
        }
    }
    return pairs >= 2;
}

//EFFECTS Returns true if hand contains three of a kind
bool PokerHand::is_threeOfKind() const {
    vector<int> rank_counts = get_rank_counts();
    
    for (int count : rank_counts) {
        if (count >= 3) {
            return true;
        }
    }
    return false;
}

//EFFECTS Returns true if hand is a straight (5 consecutive ranks)
bool PokerHand::is_straight() const {
    if (cards.size() < 5) return false;
    
    vector<Rank> ranks;
    for (const Card& card : cards) {
        ranks.push_back(card.get_rank());
    }
    
    sort(ranks.begin(), ranks.end());
    
    // Check for regular straight
    for (size_t i = 1; i < ranks.size(); ++i) {
        if (ranks[i] != ranks[i-1] + 1) {
            return false;
        }
    }
    
    return true;
}

//EFFECTS Returns true if hand is a full house (three of a kind + pair)
bool PokerHand::is_fullHouse() const {
    vector<int> rank_counts = get_rank_counts();
    
    bool has_three = false;
    bool has_pair = false;
    
    for (int count : rank_counts) {
        if (count >= 3) {
            has_three = true;
        } else if (count >= 2) {
            has_pair = true;
        }
    }
    
    return has_three && has_pair;
}

//EFFECTS Returns true if hand contains four of a kind
bool PokerHand::is_fourOfKind() const {
    vector<int> rank_counts = get_rank_counts();
    
    for (int count : rank_counts) {
        if (count >= 4) {
            return true;
        }
    }
    return false;
}

//EFFECTS Returns true if hand is a straight flush
bool PokerHand::is_straightFlush() const {
    return is_straight() && is_flush();
}

//EFFECTS Returns true if hand is a royal flush (A, K, Q, J, 10 of same suit)
bool PokerHand::is_royalFlush() const {
    if (!is_straightFlush() || cards.size() != 5) return false;
    
    vector<Rank> ranks;
    for (const Card& card : cards) {
        ranks.push_back(card.get_rank());
    }
    
    sort(ranks.begin(), ranks.end());
    
    // Check for 10, J, Q, K, A
    return ranks[0] == TEN && ranks[1] == JACK && ranks[2] == QUEEN && 
           ranks[3] == KING && ranks[4] == ACE;
}

//EFFECTS Returns true if hand contains five of a kind (Balatro special)
bool PokerHand::is_fiveOfKind() const {
    vector<int> rank_counts = get_rank_counts();
    
    for (int count : rank_counts) {
        if (count >= 5) {
            return true;
        }
    }
    return false;
}

//EFFECTS Returns true if hand is flush house (full house + flush)
bool PokerHand::is_flushHouse() const {
    return is_fullHouse() && is_flush();
}

//EFFECTS Returns true if hand is flush five (five of a kind + flush)
bool PokerHand::is_flushFive() const {
    return is_fiveOfKind() && is_flush();
}

//EFFECTS Returns count of each rank in the hand
vector<int> PokerHand::get_rank_counts() const {
    map<Rank, int> rank_map;
    
    for (const Card& card : cards) {
        rank_map[card.get_rank()]++;
    }
    
    vector<int> counts;
    for (const auto& pair : rank_map) {
        counts.push_back(pair.second);
    }
    
    return counts;
}

//EFFECTS Sets base chips and multiplier based on hand type
void PokerHand::calculate_base_values() {
    switch (type) {
        case HIGH_CARD:
            base_chips = 5; base_mult = 1; break;
        case PAIR:
            base_chips = 10; base_mult = 2; break;
        case TWO_PAIR:
            base_chips = 20; base_mult = 2; break;
        case THREE_OF_KIND:
            base_chips = 30; base_mult = 3; break;
        case STRAIGHT:
            base_chips = 30; base_mult = 4; break;
        case FLUSH:
            base_chips = 35; base_mult = 4; break;
        case FULL_HOUSE:
            base_chips = 40; base_mult = 4; break;
        case FOUR_OF_KIND:
            base_chips = 60; base_mult = 7; break;
        case STRAIGHT_FLUSH:
            base_chips = 100; base_mult = 8; break;
        case ROYAL_FLUSH:
            base_chips = 100; base_mult = 8; break;
        case FIVE_OF_KIND:
            base_chips = 120; base_mult = 12; break;
        case FLUSH_HOUSE:
            base_chips = 140; base_mult = 14; break;
        case FLUSH_FIVE:
            base_chips = 160; base_mult = 16; break;
        default:
            base_chips = 5; base_mult = 1; break;
    }
}

//EFFECTS Converts hand type enum to readable string
string hand_type_to_string(HandType type) {
    switch (type) {
        case HIGH_CARD:
            return "High Card";
        case PAIR:
            return "Pair";
        case TWO_PAIR:
            return "Two Pair";
        case THREE_OF_KIND:
            return "Three of a Kind";
        case STRAIGHT:
            return "Straight";
        case FLUSH:
            return "Flush";
        case FULL_HOUSE:
            return "Full House";
        case FOUR_OF_KIND:
            return "Four of a Kind";
        case STRAIGHT_FLUSH:
            return "Straight Flush";
        case ROYAL_FLUSH:
            return "Royal Flush";
        case FIVE_OF_KIND:
            return "Five of a Kind";
        case FLUSH_HOUSE:
            return "Flush House";
        case FLUSH_FIVE:
            return "Flush Five";
        default:
            return "Unknown";
    }
}