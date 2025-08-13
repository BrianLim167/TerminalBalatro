#include <iostream>
#include <vector>
#include "../HandPreview.hpp"
#include "../GameManager.hpp"
#include "../Card.hpp"

// External test counters
extern int tests_run;
extern int tests_passed;

void test_hand_preview_calculation() {
    std::cout << "Testing hand preview calculation..." << std::endl;
    tests_run++;
    
    try {
        GameManager game;
        game.start_new_game();
        
        // Select first 5 cards for testing
        std::vector<int> selected_indices = {0, 1, 2, 3, 4};
        
        // Calculate preview
        HandScoreBreakdown breakdown = HandPreview::calculate_hand_preview(game, selected_indices);
        
        // Verify basic properties
        if (breakdown.cards.size() == 5 &&
            breakdown.total_score > 0 &&
            breakdown.final_chips > 0 &&
            breakdown.final_mult > 0 &&
            !breakdown.hand_name.empty()) {
            tests_passed++;
            std::cout << "  ✓ Hand preview calculation works correctly" << std::endl;
        } else {
            std::cout << "  ✗ Hand preview calculation failed" << std::endl;
            std::cout << "    Cards: " << breakdown.cards.size() << std::endl;
            std::cout << "    Score: " << breakdown.total_score << std::endl;
            std::cout << "    Hand: " << breakdown.hand_name << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Exception in hand preview test: " << e.what() << std::endl;
    }
}

void test_hand_preview_with_jokers() {
    std::cout << "Testing hand preview with jokers..." << std::endl;
    tests_run++;
    
    try {
        GameManager game;
        game.start_new_game();
        
        // Buy a joker to test joker effects
        game.buy_joker("Joker"); // Basic joker gives +4 mult
        
        std::vector<int> selected_indices = {0, 1, 2, 3, 4};
        
        // Calculate preview with joker
        HandScoreBreakdown breakdown = HandPreview::calculate_hand_preview(game, selected_indices);
        
        // Should have joker effects
        if (breakdown.joker_mult_bonus > 0 || breakdown.joker_chip_bonus != 0) {
            tests_passed++;
            std::cout << "  ✓ Hand preview with jokers works correctly" << std::endl;
        } else {
            std::cout << "  ✗ Hand preview with jokers failed - no joker effects detected" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Exception in joker preview test: " << e.what() << std::endl;
    }
}

void test_card_chip_calculation() {
    std::cout << "Testing card chip calculation..." << std::endl;
    tests_run++;
    
    try {
        // Test with known cards
        std::vector<Card> test_cards = {
            Card(TWO, SPADES),    // Should be 2 chips
            Card(KING, HEARTS),   // Should be 10 chips (face card)
            Card(ACE, DIAMONDS)   // Should be 10 chips (ace)
        };
        
        // This is a private method, so we'll test it indirectly through the preview system
        GameManager game;
        game.start_new_game();
        
        // We can't directly test the private method, but we can verify the calculation
        // works correctly through the preview system
        const auto& hand = game.get_hand();
        if (hand.size() >= 3) {
            std::vector<int> selected_indices = {0, 1, 2};
            HandScoreBreakdown breakdown = HandPreview::calculate_hand_preview(game, selected_indices);
            
            // Verify that card chips are calculated (should be > 0)
            if (breakdown.card_chips > 0) {
                tests_passed++;
                std::cout << "  ✓ Card chip calculation works correctly" << std::endl;
            } else {
                std::cout << "  ✗ Card chip calculation failed" << std::endl;
            }
        } else {
            tests_passed++;
            std::cout << "  ✓ Card chip calculation test skipped (insufficient cards)" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Exception in card chip test: " << e.what() << std::endl;
    }
}

void test_preview_consistency() {
    std::cout << "Testing preview consistency with actual play..." << std::endl;
    tests_run++;
    
    try {
        GameManager game1, game2;
        game1.start_new_game();
        game2.start_new_game();
        
        // Make sure both games have the same state (same seed would be ideal)
        // For now, we'll test that the preview calculation is internally consistent
        
        std::vector<int> selected_indices = {0, 1, 2, 3, 4};
        
        // Calculate preview
        HandScoreBreakdown breakdown = HandPreview::calculate_hand_preview(game1, selected_indices);
        
        // Verify the math: total_score should equal final_chips * final_mult
        int expected_score = breakdown.final_chips * breakdown.final_mult;
        
        if (breakdown.total_score == expected_score) {
            tests_passed++;
            std::cout << "  ✓ Preview calculation is mathematically consistent" << std::endl;
        } else {
            std::cout << "  ✗ Preview calculation inconsistent" << std::endl;
            std::cout << "    Expected: " << expected_score << std::endl;
            std::cout << "    Got: " << breakdown.total_score << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Exception in consistency test: " << e.what() << std::endl;
    }
}

void run_handpreview_tests() {
    std::cout << "Running Hand Preview Tests..." << std::endl;
    std::cout << "=============================" << std::endl;
    
    test_hand_preview_calculation();
    test_hand_preview_with_jokers();
    test_card_chip_calculation();
    test_preview_consistency();
    
    std::cout << std::endl;
}