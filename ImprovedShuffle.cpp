#include "ImprovedShuffle.hpp"
#include <chrono>
#include <algorithm>

using namespace std;

// Static member definitions
mt19937 ImprovedShuffle::rng;
bool ImprovedShuffle::rng_initialized = false;

//EFFECTS Creates a properly shuffled deck using modern C++ random facilities
vector<Card> ImprovedShuffle::create_shuffled_deck() {
    vector<Card> deck;
    deck.reserve(52);
    
    // Create standard 52-card deck
    for (int suit = SPADES; suit <= DIAMONDS; ++suit) {
        for (int rank = TWO; rank <= ACE; ++rank) {
            deck.emplace_back(static_cast<Rank>(rank), static_cast<Suit>(suit));
        }
    }
    
    // Shuffle using improved algorithm
    shuffle_deck(deck);
    
    return deck;
}

//EFFECTS Shuffles an existing deck using Fisher-Yates algorithm
void ImprovedShuffle::shuffle_deck(vector<Card>& deck) {
    if (!rng_initialized) {
        initialize_rng();
    }
    
    // Use std::shuffle which implements Fisher-Yates internally
    shuffle(deck.begin(), deck.end(), rng);
}

//EFFECTS Returns a single, properly seeded random number generator
mt19937& ImprovedShuffle::get_rng() {
    if (!rng_initialized) {
        initialize_rng();
    }
    return rng;
}

//EFFECTS Initializes the global RNG with proper seeding
void ImprovedShuffle::initialize_rng() {
    // Use multiple entropy sources for better seeding
    random_device rd;
    
    // Combine multiple entropy sources
    auto now = chrono::high_resolution_clock::now();
    auto time_seed = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()).count();
    
    // Create seed sequence from multiple sources
    seed_seq seq{
        rd(), rd(), rd(), rd(),  // Hardware entropy
        static_cast<uint32_t>(time_seed),
        static_cast<uint32_t>(time_seed >> 32)
    };
    
    rng.seed(seq);
    rng_initialized = true;
}