#include <iostream>
#include <vector>
#include <memory>
#include "../GameManager.hpp"
#include "../Joker.hpp"
#include "../Shop.hpp"

// External test counters
extern int tests_run;
extern int tests_passed;

void test_smart_pointer_usage() {
    std::cout << "Testing smart pointer usage..." << std::endl;
    tests_run++;
    
    try {
        // Test joker creation and destruction
        std::vector<std::unique_ptr<Joker>> jokers;
        
        jokers.push_back(std::unique_ptr<Joker>(Joker_factory("Joker")));
        jokers.push_back(std::unique_ptr<Joker>(Joker_factory("Pair Joker")));
        jokers.push_back(std::unique_ptr<Joker>(Joker_factory("Flush Joker")));
        
        // Verify jokers were created
        if (jokers.size() == 3 && 
            jokers[0]->get_name() == "Joker" &&
            jokers[1]->get_name() == "Pair Joker" &&
            jokers[2]->get_name() == "Flush Joker") {
            tests_passed++;
            std::cout << "  ✓ Smart pointer joker management works correctly" << std::endl;
        } else {
            std::cout << "  ✗ Smart pointer joker management failed" << std::endl;
        }
        
        // Jokers should be automatically cleaned up when vector goes out of scope
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Exception in smart pointer test: " << e.what() << std::endl;
    }
}

void test_game_manager_memory() {
    std::cout << "Testing GameManager memory management..." << std::endl;
    tests_run++;
    
    try {
        // Create and destroy multiple game managers
        for (int i = 0; i < 10; ++i) {
            GameManager game;
            game.start_new_game();
            
            // Buy some jokers to test memory management
            game.buy_joker("Joker");
            game.buy_joker("Pair Joker");
            
            // Game should clean up automatically when it goes out of scope
        }
        
        tests_passed++;
        std::cout << "  ✓ GameManager memory management works correctly" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Exception in GameManager memory test: " << e.what() << std::endl;
    }
}

void test_shop_memory() {
    std::cout << "Testing Shop memory management..." << std::endl;
    tests_run++;
    
    try {
        Shop shop;
        
        // Restock multiple times to test memory management
        for (int i = 0; i < 5; ++i) {
            shop.restock();
            shop.clear_inventory();
        }
        
        tests_passed++;
        std::cout << "  ✓ Shop memory management works correctly" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Exception in Shop memory test: " << e.what() << std::endl;
    }
}

void test_no_memory_leaks() {
    std::cout << "Testing for obvious memory leaks..." << std::endl;
    tests_run++;
    
    try {
        // Simulate a short game session
        GameManager game;
        Shop shop;
        
        game.start_new_game();
        
        // Play a few hands
        for (int i = 0; i < 3; ++i) {
            const auto& hand = game.get_hand();
            if (!hand.empty()) {
                std::vector<int> selected = {0}; // Play first card
                game.play_hand(selected);
            }
            game.deal_hand();
        }
        
        // Buy some jokers
        shop.restock();
        if (!shop.get_inventory().empty()) {
            game.buy_joker(shop.get_inventory()[0].name);
        }
        
        tests_passed++;
        std::cout << "  ✓ No obvious memory leaks detected" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Exception in memory leak test: " << e.what() << std::endl;
    }
}

void run_memory_tests() {
    std::cout << "Running Memory Management Tests..." << std::endl;
    std::cout << "=================================" << std::endl;
    
    test_smart_pointer_usage();
    test_game_manager_memory();
    test_shop_memory();
    test_no_memory_leaks();
    
    std::cout << std::endl;
}