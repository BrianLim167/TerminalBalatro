#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <sstream>

#include "GameManager.hpp"
#include "Shop.hpp"
#include "Utils.hpp"
#include "Poker.hpp"
#include "Statistics.hpp"
#include "HandPreview.hpp"

using namespace std;

//EFFECTS Displays main menu and returns user choice
int show_main_menu() {
    clear_screen();
    print_centered("BALATRO C++", 60);
    print_separator();
    cout << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Statistics" << endl;
    cout << "4. Rules" << endl;  
    cout << "5. Exit" << endl;
    cout << endl;
    
    int choice = get_int_input("Choose option: ", 1, 5);
    play_game_sound();
    return choice;
}

//EFFECTS Displays game rules
void show_rules() {
    clear_screen();
    print_centered("BALATRO RULES", 60);
    print_separator();
    cout << endl;
    cout << "Goal: Beat 8 antes by scoring enough chips with poker hands" << endl;
    cout << endl;
    cout << "Each ante has 3 blinds: Small, Big, Boss" << endl;
    cout << "- Small Blind: Base requirement" << endl;
    cout << "- Big Blind: 2x requirement" << endl;
    cout << "- Boss Blind: 3x requirement" << endl;
    cout << endl;
    cout << "Each blind gives you:" << endl;
    cout << "- 4 hands to play" << endl;
    cout << "- 3 discards" << endl;
    cout << "- 8 cards in hand" << endl;
    cout << endl;
    cout << "Scoring: (Base Chips + Card Chips) x Multiplier" << endl;
    cout << "- Number cards worth face value" << endl;
    cout << "- Face cards worth 10 chips" << endl;
    cout << "- Hand types give base chips and multiplier" << endl;
    cout << endl;
    cout << "Jokers modify your scoring - buy them in the shop!" << endl;
    cout << "Earn $1 for each unused hand" << endl;
    cout << endl;
    
    wait_for_enter();
}

//EFFECTS Shows game statistics
void show_statistics() {
    Statistics stats;
    stats.load_statistics("balatro_stats.dat");
    stats.display_statistics();
}

//EFFECTS Displays current game state
void display_game_state(const GameManager& game) {
    clear_screen();
    
    // Header with game info
    print_game_stats(game.get_ante(), game.get_blind_name(),
                    game.get_current_score(), game.get_chips_needed(),
                    game.get_hands_left(), game.get_discards_left(), 
                    game.get_money());
    
    print_separator();
    
    // Display jokers if any
    const auto& jokers = game.get_jokers();
    if (!jokers.empty()) {
        cout << "Jokers:" << endl;
        for (size_t i = 0; i < jokers.size(); ++i) {
            cout << "  " << (i+1) << ". " << jokers[i]->get_name() 
                 << " - " << jokers[i]->get_description() << endl;
        }
        cout << endl;
    }
    
    // Display hand
    cout << "Your Hand:" << endl;
    print_cards_with_indices(game.get_hand());
    cout << endl;
}

//EFFECTS Handles playing a hand with preview system
void play_hand_action(GameManager& game) {
    if (game.get_hands_left() <= 0) {
        print_error("No hands remaining!");
        wait_for_enter();
        return;
    }
    
    while (true) {
        display_game_state(game);
        
        const auto& hand = game.get_hand();
        
        cout << "Select cards to play:" << endl;
        vector<int> selected = get_multiple_int_input("Enter cards to play (e.g. 1,3,5 or 2 4 7)", 5, hand.size());
        
        if (selected.empty()) {
            print_warning("No cards selected!");
            wait_for_enter();
            return;
        }
        
        // Convert to 0-based indices for preview calculation
        vector<int> zero_based_selected;
        for (int index : selected) {
            zero_based_selected.push_back(index - 1);
        }
        
        // Show preview and get confirmation
        bool confirmed = HandPreview::preview_and_confirm_hand(game, zero_based_selected);
        
        if (confirmed) {
            // User confirmed - play the hand
            int score = game.play_hand(zero_based_selected);
            
            // Show final result
            clear_screen();
            print_success("HAND PLAYED!");
            print_separator();
            
            // Capture the cards that were played for display
            vector<Card> played_cards;
            PokerHand played_hand;
            for (int index : zero_based_selected) {
                played_cards.push_back(hand[index]);
                played_hand.add_card(hand[index]);
            }
            
            // Display the hand type and cards played
            string cards_played = "";
            for (size_t i = 0; i < played_cards.size(); ++i) {
                if (i > 0) cards_played += ", ";
                stringstream ss;
                ss << played_cards[i];
                cards_played += ss.str();
            }
            
            cout << "Played: " << hand_type_to_string(played_hand.get_type()) 
                 << " (" << cards_played << ")" << endl;
            cout << "Scored: " << format_number(score) << " chips!" << endl;
            cout << endl;
            
            // Track statistics
            Statistics stats;
            stats.load_statistics("balatro_stats.dat");
            stats.record_hand_played(hand_type_to_string(played_hand.get_type()), score);
            stats.save_statistics("balatro_stats.dat");
            
            wait_for_enter();
            break;
        }
        // If not confirmed, loop back to card selection
    }
}

//EFFECTS Handles previewing a hand without playing it
void preview_hand_action(GameManager& game) {
    const auto& hand = game.get_hand();
    
    cout << "Select cards to preview:" << endl;
    vector<int> selected = get_multiple_int_input("Enter cards to preview (e.g. 1,3,5 or 2 4 7)", 5, hand.size());
    
    if (selected.empty()) {
        print_warning("No cards selected!");
        wait_for_enter();
        return;
    }
    
    // Convert to 0-based indices for preview calculation
    vector<int> zero_based_selected;
    for (int index : selected) {
        zero_based_selected.push_back(index - 1);
    }
    
    // Show preview only (no confirmation needed)
    HandScoreBreakdown breakdown = HandPreview::calculate_hand_preview(game, zero_based_selected);
    HandPreview::display_hand_preview(breakdown);
    
    cout << "This is just a preview - no hand was played." << endl;
    wait_for_enter();
}

//EFFECTS Handles discarding cards
void discard_action(GameManager& game) {
    if (game.get_discards_left() <= 0) {
        print_error("No discards remaining!");
        wait_for_enter();
        return;
    }
    
    const auto& hand = game.get_hand();
    
    vector<int> selected = get_multiple_int_input("Enter cards to discard (e.g. 1,3,5 or 2 4 7)", 
                                                 hand.size(), hand.size());
    
    if (selected.empty()) {
        print_warning("No cards selected!");
        wait_for_enter();
        return;
    }
    
    // Convert from 1-based to 0-based indexing
    for (int& index : selected) {
        index--;
    }
    
    game.discard_cards(selected);
    print_success("Discarded " + to_string(selected.size()) + " cards!");
    cout << endl;
    
    wait_for_enter();
}

//EFFECTS Shows shop interface
void show_shop(GameManager& game, Shop& shop) {
    while (true) {
        clear_screen();
        print_centered("SHOP", 60);
        print_separator();
        cout << "Money: $" << game.get_money() << endl;
        cout << "Restock Cost: $" << shop.get_restock_cost() << endl;
        cout << endl;
        
        const auto& inventory = shop.get_inventory();
        if (inventory.empty()) {
            cout << "Shop is empty! Restock to see new items." << endl;
        } else {
            cout << "Available Items:" << endl;
            for (size_t i = 0; i < inventory.size(); ++i) {
                cout << "  " << (i+1) << ". " << inventory[i];
                if (!shop.can_afford(i, game.get_money())) {
                    cout << " (Can't afford)";
                }
                cout << endl;
            }
        }
        
        cout << endl;
        cout << "1-" << inventory.size() << ". Buy item" << endl;
        cout << (inventory.size() + 1) << ". Restock ($" << shop.get_restock_cost() << ")" << endl;
        cout << (inventory.size() + 2) << ". Leave shop" << endl;
        
        int choice = get_int_input("Choose option: ", 1, inventory.size() + 2);
        
        if (choice <= static_cast<int>(inventory.size())) {
            // Buy item
            int item_index = choice - 1;
            if (shop.can_afford(item_index, game.get_money())) {
                if (game.buy_joker(inventory[item_index].name)) {
                    print_success("Bought " + inventory[item_index].name + "!");
                    
                    // Track joker purchase
                    Statistics stats;
                    stats.load_statistics("balatro_stats.dat");
                    stats.record_joker_bought(inventory[item_index].name, inventory[item_index].cost);
                    stats.save_statistics("balatro_stats.dat");
                    
                    shop.buy_item(item_index); // Remove from shop
                } else {
                    print_error("Failed to buy item!");
                }
            } else {
                print_error("Not enough money!");
            }
            wait_for_enter();
        } else if (choice == static_cast<int>(inventory.size()) + 1) {
            // Restock
            if (game.spend_money(shop.get_restock_cost())) {
                shop.restock();
                shop.increase_restock_cost();
                print_success("Shop restocked!");
            } else {
                print_error("Not enough money to restock!");
            }
            wait_for_enter();
        } else {
            // Leave shop
            break;
        }
    }
}

//EFFECTS Main gameplay loop for a single blind
bool play_blind(GameManager& game, Shop& shop) {
    while (game.get_hands_left() > 0 && !game.is_blind_complete()) {
        display_game_state(game);
        
        cout << "Actions:" << endl;
        cout << "1. Play hand" << endl;
        cout << "2. Preview hand (no commitment)" << endl;
        cout << "3. Discard cards" << endl;
        cout << "4. View jokers" << endl;
        cout << "5. Save game" << endl;
        cout << "6. Quit game" << endl;
        
        int choice = get_int_input("Choose action: ", 1, 6);
        
        switch (choice) {
            case 1:
                play_hand_action(game);
                break;
            case 2:
                preview_hand_action(game);
                break;
            case 3:
                discard_action(game);
                break;
            case 4:
                // View jokers (already shown in display)
                wait_for_enter();
                break;
            case 5:
                if (game.save_game("balatro_save.dat")) {
                    print_success("Game saved successfully!");
                } else {
                    print_error("Failed to save game!");
                }
                wait_for_enter();
                break;
            case 6:
                return false; // Quit
        }
    }
    
    // Check if blind was completed
    if (game.is_blind_complete()) {
        clear_screen();
        print_success("Blind completed!");
        cout << "Score: " << format_number(game.get_current_score()) 
             << " / " << format_number(game.get_chips_needed()) << endl;
        cout << "Earned $" << game.get_hands_left() << " for unused hands" << endl;
        wait_for_enter();
        
        // Show shop between blinds
        show_shop(game, shop);
        
        return true;
    } else {
        clear_screen();
        print_error("Blind failed!");
        cout << "Score: " << format_number(game.get_current_score()) 
             << " / " << format_number(game.get_chips_needed()) << endl;
        wait_for_enter();
        return false;
    }
}

//EFFECTS Loads and plays a saved game
void load_and_play_game() {
    GameManager game;
    
    if (game.load_game("balatro_save.dat")) {
        print_success("Game loaded successfully!");
        wait_for_enter();
        
        Shop shop;
        
        while (game.get_game_state() == PLAYING) {
            bool blind_completed = play_blind(game, shop);
            
            if (!blind_completed) {
                // Game over
                clear_screen();
                print_error("GAME OVER!");
                cout << "Reached Ante " << game.get_ante() 
                     << " - " << game.get_blind_name() << endl;
                wait_for_enter();
                break;
            }
            
            // Advance to next blind
            if (!game.advance_blind()) {
                // Game completed
                clear_screen();
                print_success("VICTORY!");
                cout << "Congratulations! You beat all 8 antes!" << endl;
                wait_for_enter();
                break;
            }
        }
    } else {
        print_error("Could not load save file!");
        wait_for_enter();
    }
}

//EFFECTS Main game loop
void play_game() {
    GameManager game;
    Shop shop;
    
    game.start_new_game();
    
    while (game.get_game_state() == PLAYING) {
        bool blind_completed = play_blind(game, shop);
        
        if (!blind_completed) {
            // Game over
            clear_screen();
            print_error("GAME OVER!");
            cout << "Reached Ante " << game.get_ante() 
                 << " - " << game.get_blind_name() << endl;
            
            // Track game result
            Statistics stats;
            stats.load_statistics("balatro_stats.dat");
            stats.record_game(false, game.get_ante(), game.get_current_score(), game.get_money());
            stats.save_statistics("balatro_stats.dat");
            
            wait_for_enter();
            break;
        }
        
        // Advance to next blind
        if (!game.advance_blind()) {
            // Game completed
            clear_screen();
            print_success("VICTORY!");
            cout << "Congratulations! You beat all 8 antes!" << endl;
            
            // Track game victory
            Statistics stats;
            stats.load_statistics("balatro_stats.dat");
            stats.record_game(true, 8, game.get_current_score(), game.get_money());
            stats.save_statistics("balatro_stats.dat");
            
            wait_for_enter();
            break;
        }
    }
}

//EFFECTS Main function
int main() {
    seed_random();
    
    while (true) {
        int choice = show_main_menu();
        
        switch (choice) {
            case 1:
                play_game_sound();
                play_game();
                break;
            case 2:
                load_and_play_game();
                break;
            case 3:
                show_statistics();
                break;
            case 4:
                show_rules();
                break;
            case 5:
                cout << "Thanks for playing!" << endl;
                return 0;
        }
    }
    
    return 0;
}