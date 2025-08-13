#ifndef HANDPREVIEW_HPP
#define HANDPREVIEW_HPP

#include <vector>
#include "Card.hpp"
#include "Poker.hpp"
#include "GameManager.hpp"

struct HandScoreBreakdown {
    HandType hand_type;
    std::vector<Card> cards;
    int base_chips;
    int card_chips;
    int base_mult;
    int joker_mult_bonus;
    int joker_chip_bonus;
    int final_chips;
    int final_mult;
    int total_score;
    std::string hand_name;
    std::vector<std::string> joker_effects;
};

class HandPreview {
public:
    //EFFECTS Calculates the scoring breakdown for a potential hand
    //REQUIRES selected_indices contains valid hand indices
    static HandScoreBreakdown calculate_hand_preview(
        const GameManager& game, 
        const std::vector<int>& selected_indices);
    
    //EFFECTS Displays the hand preview in a formatted way
    static void display_hand_preview(const HandScoreBreakdown& breakdown);
    
    //EFFECTS Shows preview and asks for confirmation to play the hand
    //RETURNS true if user confirms, false if they want to go back
    static bool preview_and_confirm_hand(
        const GameManager& game, 
        const std::vector<int>& selected_indices);

private:
    //EFFECTS Calculates card chip values (face cards = 10, others = face value)
    static int calculate_card_chips(const std::vector<Card>& cards);
    
    //EFFECTS Formats the joker effects for display
    static std::vector<std::string> format_joker_effects(
        const std::vector<std::unique_ptr<Joker>>& jokers,
        const HandInfo& hand_info);
};

#endif // HANDPREVIEW_HPP