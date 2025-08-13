#include "Benchmark.hpp"
#include "Card.hpp"
#include "Poker.hpp"
#include "GameManager.hpp"
#include "Utils.hpp"
#include <vector>
#include <random>

void benchmark_card_operations() {
    std::cout << "Running Card Operations Benchmarks..." << std::endl;
    std::vector<Benchmark::Result> results;
    
    // Benchmark card creation
    results.push_back(BENCHMARK("Card Creation", {
        Card card(ACE, SPADES);
    }, 10000));
    
    // Benchmark card comparison
    Card card1(KING, HEARTS);
    Card card2(QUEEN, DIAMONDS);
    results.push_back(BENCHMARK("Card Comparison", {
        bool result = card1 > card2;
        (void)result; // Suppress unused variable warning
    }, 10000));
    
    Benchmark::print_results(results);
}

void benchmark_poker_hands() {
    std::cout << "Running Poker Hand Benchmarks..." << std::endl;
    std::vector<Benchmark::Result> results;
    
    // Create test hands
    std::vector<Card> royal_flush = {
        Card(TEN, SPADES), Card(JACK, SPADES), Card(QUEEN, SPADES),
        Card(KING, SPADES), Card(ACE, SPADES)
    };
    
    std::vector<Card> pair_hand = {
        Card(ACE, SPADES), Card(ACE, HEARTS), Card(KING, DIAMONDS),
        Card(QUEEN, CLUBS), Card(JACK, SPADES)
    };
    
    // Benchmark hand evaluation
    results.push_back(BENCHMARK("Royal Flush Evaluation", {
        PokerHand hand(royal_flush);
        hand.evaluate_hand();
    }, 1000));
    
    results.push_back(BENCHMARK("Pair Hand Evaluation", {
        PokerHand hand(pair_hand);
        hand.evaluate_hand();
    }, 1000));
    
    // Benchmark score calculation
    PokerHand test_hand(pair_hand);
    test_hand.evaluate_hand();
    results.push_back(BENCHMARK("Score Calculation", {
        int score = test_hand.calculate_score();
        (void)score;
    }, 5000));
    
    Benchmark::print_results(results);
}

void benchmark_deck_operations() {
    std::cout << "Running Deck Operations Benchmarks..." << std::endl;
    std::vector<Benchmark::Result> results;
    
    // Benchmark deck creation and shuffling
    results.push_back(BENCHMARK("Deck Creation & Shuffle", {
        std::vector<Card> deck;
        for (int rank = TWO; rank <= ACE; ++rank) {
            for (int suit = SPADES; suit <= DIAMONDS; ++suit) {
                deck.emplace_back(static_cast<Rank>(rank), static_cast<Suit>(suit));
            }
        }
        
        // Simple shuffle
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deck.begin(), deck.end(), g);
    }, 1000));
    
    Benchmark::print_results(results);
}

void benchmark_string_operations() {
    std::cout << "Running String Operations Benchmarks..." << std::endl;
    std::vector<Benchmark::Result> results;
    
    // Benchmark number formatting
    results.push_back(BENCHMARK("Number Formatting", {
        std::string formatted = format_number(1234567);
        (void)formatted;
    }, 5000));
    
    // Benchmark hand type to string conversion
    results.push_back(BENCHMARK("Hand Type to String", {
        std::string hand_str = hand_type_to_string(ROYAL_FLUSH);
        (void)hand_str;
    }, 5000));
    
    Benchmark::print_results(results);
}

void benchmark_game_simulation() {
    std::cout << "Running Game Simulation Benchmark..." << std::endl;
    std::vector<Benchmark::Result> results;
    
    // Benchmark game initialization
    results.push_back(BENCHMARK("Game Initialization", {
        GameManager game;
        game.start_new_game();
    }, 100));
    
    // Benchmark hand dealing
    GameManager game;
    game.start_new_game();
    results.push_back(BENCHMARK("Hand Dealing", {
        game.deal_hand();
    }, 500));
    
    Benchmark::print_results(results);
}

int main() {
    seed_random();
    
    std::cout << "Balatro C++ Performance Benchmarks" << std::endl;
    std::cout << "===================================" << std::endl;
    
    benchmark_card_operations();
    benchmark_poker_hands();
    benchmark_deck_operations();
    benchmark_string_operations();
    benchmark_game_simulation();
    
    std::cout << "\nBenchmarking complete!" << std::endl;
    std::cout << "Use these results to identify performance bottlenecks." << std::endl;
    
    return 0;
}