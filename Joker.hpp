#ifndef JOKER_HPP
#define JOKER_HPP

#include <string>
#include <vector>
#include "Card.hpp"
#include "Poker.hpp"

struct HandInfo {
    HandType type;
    std::vector<Card> cards;
    int base_chips;
    int base_mult;
};

enum JokerRarity {
    COMMON = 1,
    UNCOMMON = 2,
    RARE = 3,
    LEGENDARY = 4
};

class Joker {
public:
    //EFFECTS Returns the name of this joker
    virtual std::string get_name() const = 0;
    
    //EFFECTS Returns the description of this joker's effect
    virtual std::string get_description() const = 0;
    
    //EFFECTS Returns the rarity of this joker
    virtual JokerRarity get_rarity() const = 0;
    
    //EFFECTS Returns the cost of this joker in the shop
    virtual int get_cost() const = 0;
    
    //EFFECTS Applies this joker's effect to the hand scoring
    //MODIFIES chips and mult may be modified based on hand and cards
    virtual void apply_effect(const HandInfo& hand, int& chips, int& mult) = 0;
    
    //EFFECTS Virtual destructor for proper cleanup
    virtual ~Joker() {}
};

// Concrete Joker implementations

class BasicJoker : public Joker {
public:
    //EFFECTS Returns "Joker"
    std::string get_name() const override;
    
    //EFFECTS Returns description of +4 mult effect
    std::string get_description() const override;
    
    //EFFECTS Returns COMMON rarity
    JokerRarity get_rarity() const override;
    
    //EFFECTS Returns cost of 2
    int get_cost() const override;
    
    //EFFECTS Adds +4 to mult
    void apply_effect(const HandInfo& hand, int& chips, int& mult) override;
};

class PairJoker : public Joker {
public:
    //EFFECTS Returns "Pair Joker"
    std::string get_name() const override;
    
    //EFFECTS Returns description of pair bonus effect
    std::string get_description() const override;
    
    //EFFECTS Returns UNCOMMON rarity
    JokerRarity get_rarity() const override;
    
    //EFFECTS Returns cost of 5
    int get_cost() const override;
    
    //EFFECTS Adds +2 mult for each pair in hand
    void apply_effect(const HandInfo& hand, int& chips, int& mult) override;
};

class FlushJoker : public Joker {
public:
    //EFFECTS Returns "Flush Joker"
    std::string get_name() const override;
    
    //EFFECTS Returns description of flush bonus effect
    std::string get_description() const override;
    
    //EFFECTS Returns UNCOMMON rarity
    JokerRarity get_rarity() const override;
    
    //EFFECTS Returns cost of 6
    int get_cost() const override;
    
    //EFFECTS Adds +3 mult for each card in flush
    void apply_effect(const HandInfo& hand, int& chips, int& mult) override;
};

class FaceCardJoker : public Joker {
public:
    //EFFECTS Returns "Face Card Joker"
    std::string get_name() const override;
    
    //EFFECTS Returns description of face card bonus effect
    std::string get_description() const override;
    
    //EFFECTS Returns COMMON rarity
    JokerRarity get_rarity() const override;
    
    //EFFECTS Returns cost of 4
    int get_cost() const override;
    
    //EFFECTS Adds +2 mult for each face card (Jack, Queen, King, Ace)
    void apply_effect(const HandInfo& hand, int& chips, int& mult) override;
};

//EFFECTS Creates and returns a pointer to a Joker of the specified type
//REQUIRES name is a valid joker name ("Joker", "Pair Joker", etc.)
Joker* Joker_factory(const std::string& name);

//EFFECTS Prints JokerRarity to stream
std::ostream& operator<<(std::ostream& os, JokerRarity rarity);

#endif // JOKER_HPP