#include "Joker.hpp"
#include <iostream>
#include <cassert>

using namespace std;

// BasicJoker implementation

//EFFECTS Returns "Joker"
string BasicJoker::get_name() const {
    return "Joker";
}

//EFFECTS Returns description of +4 mult effect
string BasicJoker::get_description() const {
    return "+4 Mult";
}

//EFFECTS Returns COMMON rarity
JokerRarity BasicJoker::get_rarity() const {
    return COMMON;
}

//EFFECTS Returns cost of 2
int BasicJoker::get_cost() const {
    return 2;
}

//EFFECTS Adds +4 to mult
void BasicJoker::apply_effect(const HandInfo& hand, int& chips, int& mult) {
    (void)hand; (void)chips; // Suppress unused parameter warnings
    mult += 4;
}

// PairJoker implementation

//EFFECTS Returns "Pair Joker"
string PairJoker::get_name() const {
    return "Pair Joker";
}

//EFFECTS Returns description of pair bonus effect
string PairJoker::get_description() const {
    return "+2 Mult per pair";
}

//EFFECTS Returns UNCOMMON rarity
JokerRarity PairJoker::get_rarity() const {
    return UNCOMMON;
}

//EFFECTS Returns cost of 5
int PairJoker::get_cost() const {
    return 5;
}

//EFFECTS Adds +2 mult for each pair in hand
void PairJoker::apply_effect(const HandInfo& hand, int& chips, int& mult) {
    (void)chips; // Suppress unused parameter warning
    if (hand.type == PAIR || hand.type == TWO_PAIR || hand.type == FULL_HOUSE) {
        if (hand.type == PAIR || hand.type == FULL_HOUSE) {
            mult += 2;  // One pair
        } else if (hand.type == TWO_PAIR) {
            mult += 4;  // Two pairs
        }
    }
}

// FlushJoker implementation

//EFFECTS Returns "Flush Joker"
string FlushJoker::get_name() const {
    return "Flush Joker";
}

//EFFECTS Returns description of flush bonus effect
string FlushJoker::get_description() const {
    return "+3 Mult per card if poker hand is a Flush";
}

//EFFECTS Returns UNCOMMON rarity
JokerRarity FlushJoker::get_rarity() const {
    return UNCOMMON;
}

//EFFECTS Returns cost of 6
int FlushJoker::get_cost() const {
    return 6;
}

//EFFECTS Adds +3 mult for each card in flush
void FlushJoker::apply_effect(const HandInfo& hand, int& chips, int& mult) {
    (void)chips; // Suppress unused parameter warning
    if (hand.type == FLUSH || hand.type == STRAIGHT_FLUSH || hand.type == ROYAL_FLUSH || 
        hand.type == FLUSH_HOUSE || hand.type == FLUSH_FIVE) {
        mult += 3 * hand.cards.size();
    }
}

// FaceCardJoker implementation

//EFFECTS Returns "Face Card Joker"
string FaceCardJoker::get_name() const {
    return "Face Card Joker";
}

//EFFECTS Returns description of face card bonus effect
string FaceCardJoker::get_description() const {
    return "+2 Mult per face card held";
}

//EFFECTS Returns COMMON rarity
JokerRarity FaceCardJoker::get_rarity() const {
    return COMMON;
}

//EFFECTS Returns cost of 4
int FaceCardJoker::get_cost() const {
    return 4;
}

//EFFECTS Adds +2 mult for each face card (Jack, Queen, King, Ace)
void FaceCardJoker::apply_effect(const HandInfo& hand, int& chips, int& mult) {
    (void)chips; // Suppress unused parameter warning
    int face_cards = 0;
    for (const Card& card : hand.cards) {
        if (card.is_face_or_ace()) {
            face_cards++;
        }
    }
    mult += 2 * face_cards;
}

//EFFECTS Creates and returns a pointer to a Joker of the specified type
//REQUIRES name is a valid joker name ("Joker", "Pair Joker", etc.)
Joker* Joker_factory(const string& name) {
    if (name == "Joker") {
        return new BasicJoker();
    } else if (name == "Pair Joker") {
        return new PairJoker();
    } else if (name == "Flush Joker") {
        return new FlushJoker();
    } else if (name == "Face Card Joker") {
        return new FaceCardJoker();
    } else {
        assert(false); // Invalid joker name
        return nullptr;
    }
}

//EFFECTS Prints JokerRarity to stream
ostream& operator<<(ostream& os, JokerRarity rarity) {
    switch (rarity) {
        case COMMON:
            os << "Common";
            break;
        case UNCOMMON:
            os << "Uncommon";
            break;
        case RARE:
            os << "Rare";
            break;
        case LEGENDARY:
            os << "Legendary";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}