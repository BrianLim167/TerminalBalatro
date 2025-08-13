#ifndef IMPROVEDSHUFFLE_HPP
#define IMPROVEDSHUFFLE_HPP

#include <vector>
#include <random>
#include "Card.hpp"

class ImprovedShuffle {
public:
    //EFFECTS Creates a properly shuffled deck using modern C++ random facilities
    static std::vector<Card> create_shuffled_deck();
    
    //EFFECTS Shuffles an existing deck using Fisher-Yates algorithm
    static void shuffle_deck(std::vector<Card>& deck);
    
    //EFFECTS Returns a single, properly seeded random number generator
    static std::mt19937& get_rng();

private:
    //EFFECTS Initializes the global RNG with proper seeding
    static void initialize_rng();
    
    static std::mt19937 rng;
    static bool rng_initialized;
};

#endif // IMPROVEDSHUFFLE_HPP