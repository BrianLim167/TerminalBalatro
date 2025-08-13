#include "HandPreview.hpp"
#include "Utils.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

//EFFECTS Calculates the scoring breakdown for a potential hand
HandScoreBreakdown HandPreview::calculate_hand_preview(
    const GameManager& game, 
    const vector<int>& selected_indices) {
    
    HandScoreBreakdown breakdown;
    const auto& hand = game.get_hand();
    
    // Build the poker hand from selected cards
    PokerHand poker_hand;
    for (int index : selected_indices) {
        Card card = hand[index];
        poker_hand.add_card(card);
        breakdown.cards.push_back(card);
    }
    
    // Evaluate the hand
    breakdown.hand_type = poker_hand.evaluate_hand();
    breakdown.hand_name = hand_type_to_string(breakdown.hand_type);
    breakdown.base_chips = poker_hand.get_base_chips();
    breakdown.base_mult = poker_hand.get_base_mult();
    breakdown.card_chips = calculate_card_chips(breakdown.cards);
    
    // Calculate joker effects
    HandInfo hand_info;
    hand_info.type = breakdown.hand_type;
    hand_info.cards = breakdown.cards;
    hand_info.base_chips = breakdown.base_chips;
    hand_info.base_mult = breakdown.base_mult;
    
    int chips = breakdown.base_chips + breakdown.card_chips;
    int mult = breakdown.base_mult;
    
    // Apply joker effects (simulate what GameManager::play_hand does)
    const auto& jokers = game.get_jokers();
    breakdown.joker_chip_bonus = 0;
    breakdown.joker_mult_bonus = 0;
    
    int original_chips = chips;
    int original_mult = mult;
    
    for (const auto& joker : jokers) {
        joker->apply_effect(hand_info, chips, mult);
    }
    
    breakdown.joker_chip_bonus = chips - original_chips;
    breakdown.joker_mult_bonus = mult - original_mult;
    breakdown.final_chips = chips;
    breakdown.final_mult = mult;
    breakdown.total_score = chips * mult;
    
    // Format joker effects for display
    breakdown.joker_effects = format_joker_effects(jokers, hand_info);
    
    return breakdown;
}

//EFFECTS Displays the hand preview in a formatted way
void HandPreview::display_hand_preview(const HandScoreBreakdown& breakdown) {
    clear_screen();
    print_centered("HAND PREVIEW", 60);
    print_separator();
    cout << endl;
    
    // Show selected cards
    cout << "Selected Cards:" << endl;
    print_cards_with_indices(breakdown.cards);
    cout << endl;
    
    // Show hand type
    print_success("Hand Type: " + breakdown.hand_name);
    cout << endl;
    
    // Scoring breakdown
    cout << "Scoring Breakdown:" << endl;
    cout << "==================" << endl;
    
    // Base scoring
    cout << "Base Chips (" << breakdown.hand_name << "): " 
         << format_number(breakdown.base_chips) << endl;
    cout << "Card Chips: " << format_number(breakdown.card_chips) << endl;
    
    if (breakdown.joker_chip_bonus != 0) {
        cout << "Joker Chip Bonus: ";
        if (breakdown.joker_chip_bonus > 0) {
            cout << "+";
        }
        cout << format_number(breakdown.joker_chip_bonus) << endl;
    }
    
    cout << "Total Chips: " << format_number(breakdown.final_chips) << endl;
    cout << endl;
    
    // Multiplier breakdown
    cout << "Base Multiplier (" << breakdown.hand_name << "): " 
         << breakdown.base_mult << "x" << endl;
    
    if (breakdown.joker_mult_bonus != 0) {
        cout << "Joker Multiplier Bonus: ";
        if (breakdown.joker_mult_bonus > 0) {
            cout << "+";
        }
        cout << breakdown.joker_mult_bonus << "x" << endl;
    }
    
    cout << "Total Multiplier: " << breakdown.final_mult << "x" << endl;
    cout << endl;
    
    // Final calculation
    cout << "Final Score: " << format_number(breakdown.final_chips) 
         << " × " << breakdown.final_mult << " = ";
    print_success(format_number(breakdown.total_score) + " chips");
    cout << endl;
    
    // Show joker effects if any
    if (!breakdown.joker_effects.empty()) {
        cout << "Active Joker Effects:" << endl;
        for (const string& effect : breakdown.joker_effects) {
            cout << "  • " << effect << endl;
        }
        cout << endl;
    }
}

//EFFECTS Shows preview and asks for confirmation to play the hand
bool HandPreview::preview_and_confirm_hand(
    const GameManager& game, 
    const vector<int>& selected_indices) {
    
    HandScoreBreakdown breakdown = calculate_hand_preview(game, selected_indices);
    display_hand_preview(breakdown);
    
    cout << "Actions:" << endl;
    cout << "1. Play this hand" << endl;
    cout << "2. Go back and select different cards" << endl;
    cout << endl;
    
    int choice = get_int_input("Choose action: ", 1, 2);
    
    return choice == 1;
}

//EFFECTS Calculates card chip values (face cards = 10, others = face value)
int HandPreview::calculate_card_chips(const vector<Card>& cards) {
    int total = 0;
    for (const Card& card : cards) {
        Rank rank = card.get_rank();
        if (card.is_face_or_ace()) {
            total += 10; // Face cards and Ace worth 10 chips
        } else {
            total += static_cast<int>(rank) + 2; // TWO=0 -> 2 chips, THREE=1 -> 3 chips, etc.
        }
    }
    return total;
}

//EFFECTS Formats the joker effects for display
vector<string> HandPreview::format_joker_effects(
    const vector<unique_ptr<Joker>>& jokers,
    const HandInfo& hand_info) {
    
    vector<string> effects;
    
    for (const auto& joker : jokers) {
        // Simulate the effect to see what it does
        int test_chips = hand_info.base_chips;
        int test_mult = hand_info.base_mult;
        int original_chips = test_chips;
        int original_mult = test_mult;
        
        joker->apply_effect(hand_info, test_chips, test_mult);
        
        string effect_desc = joker->get_name() + ": ";
        
        bool has_effect = false;
        if (test_chips != original_chips) {
            int chip_bonus = test_chips - original_chips;
            if (chip_bonus > 0) {
                effect_desc += "+" + format_number(chip_bonus) + " chips";
            } else {
                effect_desc += format_number(chip_bonus) + " chips";
            }
            has_effect = true;
        }
        
        if (test_mult != original_mult) {
            int mult_bonus = test_mult - original_mult;
            if (has_effect) effect_desc += ", ";
            if (mult_bonus > 0) {
                effect_desc += "+" + to_string(mult_bonus) + "x mult";
            } else {
                effect_desc += to_string(mult_bonus) + "x mult";
            }
            has_effect = true;
        }
        
        if (has_effect) {
            effects.push_back(effect_desc);
        }
    }
    
    return effects;
}