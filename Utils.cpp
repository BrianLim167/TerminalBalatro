#include "Utils.hpp"
#include "ImprovedShuffle.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <set>
#include <random>
#include <ctime>
#include <limits>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;

//EFFECTS Clears the console screen
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//EFFECTS Waits for user to press Enter
void wait_for_enter() {
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl;
}

//EFFECTS Prints a horizontal line separator
void print_separator() {
    cout << string(60, '-') << endl;
}

//EFFECTS Prints centered text with padding
void print_centered(const string& text, int width) {
    if (static_cast<int>(text.length()) >= width) {
        cout << text << endl;
        return;
    }
    
    int padding = (width - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

//EFFECTS Gets integer input from user with validation
//REQUIRES min <= max
int get_int_input(const string& prompt, int min, int max) {
    int value;
    
    while (true) {
        cout << prompt;
        
        if (cin >> value) {
            if (value >= min && value <= max) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
                return value;
            } else {
                print_error("Please enter a number between " + to_string(min) + 
                           " and " + to_string(max));
            }
        } else {
            // Check if we've reached EOF (like from piped input)
            if (cin.eof()) {
                print_error("No more input available");
                exit(1); // Exit gracefully instead of infinite loop
            }
            
            print_error("Please enter a valid number");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

//EFFECTS Gets yes/no input from user, returns true for yes
bool get_yes_no_input(const string& prompt) {
    string input;
    
    while (true) {
        cout << prompt << " (y/n): ";
        getline(cin, input);
        
        if (!input.empty()) {
            char first_char = tolower(input[0]);
            if (first_char == 'y') {
                return true;
            } else if (first_char == 'n') {
                return false;
            }
        }
        
        print_error("Please enter 'y' for yes or 'n' for no");
    }
}

//EFFECTS Gets multiple integer inputs (for selecting cards)
//REQUIRES max_selections > 0 and available_count > 0
vector<int> get_multiple_int_input(const string& prompt, int max_selections, int available_count) {
    vector<int> selections;
    string input;
    
    cout << prompt << " (1-" << available_count << ", max " << max_selections << "): ";
    getline(cin, input);
    
    if (input.empty()) {
        return selections;
    }
    
    // Parse input with both spaces and commas as delimiters
    stringstream ss;
    for (char c : input) {
        if (c == ',' || c == ' ') {
            ss << ' '; // Replace commas with spaces for uniform parsing
        } else {
            ss << c;
        }
    }
    
    string token;
    stringstream parsed_ss(ss.str());
    
    while (parsed_ss >> token && selections.size() < static_cast<size_t>(max_selections)) {
        if (token.empty()) continue;
        
        try {
            int value = stoi(token);
            if (value >= 1 && value <= available_count) {
                selections.push_back(value);
            } else {
                print_warning("Ignoring invalid card number: " + token + " (must be 1-" + to_string(available_count) + ")");
            }
        } catch (const invalid_argument&) {
            print_warning("Ignoring non-numeric input: " + token);
        } catch (const out_of_range&) {
            print_warning("Ignoring out-of-range number: " + token);
        }
    }
    
    return clean_indices(selections);
}

//EFFECTS Prints a vector of cards with indices
void print_cards_with_indices(const vector<Card>& cards) {
    print_ascii_cards_horizontal(cards);
}

//EFFECTS Prints game stats in a formatted way
void print_game_stats(int ante, const string& blind_name, 
                     int score, int needed, int hands, int discards, int money) {
    cout << left;
    cout << setw(20) << ("Ante " + to_string(ante)) 
         << setw(20) << blind_name 
         << "Money: $" << money << endl;
    
    cout << setw(25) << ("Score: " + format_number(score) + " / " + format_number(needed))
         << setw(15) << ("Hands: " + to_string(hands))
         << "Discards: " << discards << endl;
}

//EFFECTS Formats large numbers with commas (e.g., 1000 -> "1,000")
string format_number(int number) {
    if (number == 0) return "0";
    
    string result;
    string num_str = to_string(abs(number));
    int count = 0;
    
    for (int i = num_str.length() - 1; i >= 0; --i) {
        if (count > 0 && count % 3 == 0) {
            result = ',' + result;
        }
        result = num_str[i] + result;
        count++;
    }
    
    if (number < 0) {
        result = '-' + result;
    }
    
    return result;
}

//EFFECTS Returns a string with specified number of spaces
string spaces(int count) {
    return string(max(0, count), ' ');
}

//EFFECTS Validates that indices are in valid range
//REQUIRES indices is not empty
bool validate_indices(const vector<int>& indices, int max_valid) {
    for (int index : indices) {
        if (index < 1 || index > max_valid) {
            return false;
        }
    }
    return true;
}

//EFFECTS Removes duplicates from vector and sorts it
vector<int> clean_indices(vector<int> indices) {
    if (indices.empty()) {
        return indices;
    }
    
    // Remove duplicates by converting to set and back
    set<int> unique_indices(indices.begin(), indices.end());
    vector<int> result(unique_indices.begin(), unique_indices.end());
    
    // Vector is already sorted due to set properties
    return result;
}

//EFFECTS Prints error message in red (if terminal supports colors)
void print_error(const string& message) {
    // ANSI color codes: Red text
    cout << "\033[31m" << "ERROR: " << message << "\033[0m" << endl;
    play_error_sound();
}

//EFFECTS Prints success message in green (if terminal supports colors)
void print_success(const string& message) {
    // ANSI color codes: Green text
    cout << "\033[32m" << message << "\033[0m" << endl;
    play_success_sound();
}

//EFFECTS Prints warning message in yellow (if terminal supports colors)
void print_warning(const string& message) {
    // ANSI color codes: Yellow text
    cout << "\033[33m" << "WARNING: " << message << "\033[0m" << endl;
    play_warning_sound();
}

//EFFECTS Returns random integer between min and max (inclusive)
int random_int(int min, int max) {
    auto& gen = ImprovedShuffle::get_rng();
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

//EFFECTS Seeds random number generator with current time
void seed_random() {
    srand(static_cast<unsigned int>(time(nullptr)));
}

//EFFECTS Returns ASCII art representation of a single card as vector of strings
vector<string> get_ascii_card(const Card& card) {
    vector<string> card_art(3); // 3 lines tall
    
    // Get rank and suit strings
    string rank_str;
    Rank rank = card.get_rank();
    
    // Convert rank to display string
    switch (rank) {
        case TWO: rank_str = "2"; break;
        case THREE: rank_str = "3"; break;
        case FOUR: rank_str = "4"; break;
        case FIVE: rank_str = "5"; break;
        case SIX: rank_str = "6"; break;
        case SEVEN: rank_str = "7"; break;
        case EIGHT: rank_str = "8"; break;
        case NINE: rank_str = "9"; break;
        case TEN: rank_str = "10"; break;
        case JACK: rank_str = "J"; break;
        case QUEEN: rank_str = "Q"; break;
        case KING: rank_str = "K"; break;
        case ACE: rank_str = "A"; break;
    }
    
    // Get suit symbol and color
    string suit_symbol;
    bool is_red = false;
    Suit suit = card.get_suit();
    
    switch (suit) {
        case SPADES:
            suit_symbol = "♠";
            is_red = false;
            break;
        case HEARTS:
            suit_symbol = "♥";
            is_red = true;
            break;
        case CLUBS:
            suit_symbol = "♣";
            is_red = false;
            break;
        case DIAMONDS:
            suit_symbol = "♦";
            is_red = true;
            break;
    }
    
    // Create card art (5 characters wide)
    card_art[0] = "┌─────┐";
    
    // Middle line with rank and suit - make all cards consistent
    string final_content;
    if (rank == TEN) {
        final_content = "10" + suit_symbol + "  ";
    } else {
        final_content = rank_str + " " + suit_symbol + "  ";
    }

    if (is_red) {
        card_art[1] = "│\033[31m" + final_content + "\033[0m│";
    } else {
        card_art[1] = "│" + final_content + "│";
    }
    
    card_art[2] = "└─────┘";
    
    return card_art;
}

//EFFECTS Prints a vector of cards as ASCII art in horizontal layout
void print_ascii_cards_horizontal(const vector<Card>& cards) {
    if (cards.empty()) {
        cout << "No cards" << endl;
        return;
    }
    
    const int CARDS_PER_ROW = 10;
    
    for (size_t start = 0; start < cards.size(); start += CARDS_PER_ROW) {
        size_t end = min(start + CARDS_PER_ROW, cards.size());
        
        // Get ASCII art for this row of cards
        vector<vector<string>> row_cards;
        for (size_t i = start; i < end; ++i) {
            row_cards.push_back(get_ascii_card(cards[i]));
        }
        
        // Print each line of the ASCII art
        for (int line = 0; line < 3; ++line) {
            for (size_t i = 0; i < row_cards.size(); ++i) {
                cout << row_cards[i][line];
                if (i < row_cards.size() - 1) {
                    cout << " "; // Space between cards
                }
            }
            cout << endl;
        }
        
        // Print card numbers below
        for (size_t i = start; i < end; ++i) {
            cout << "   " << (i + 1) << "   ";
            if (i < end - 1) {
                cout << " "; // Space between numbers
            }
        }
        cout << endl;
        
        // Add extra space between rows
        if (end < cards.size()) {
            cout << endl;
        }
    }
}

//EFFECTS Plays sound for successful actions (success beep)
void play_success_sound() {
#ifdef _WIN32
    Beep(800, 200);  // Higher pitch, short duration
#else
    cout << "\a" << flush;  // Bell character
#endif
}

//EFFECTS Plays sound for errors or failures (error beep)
void play_error_sound() {
#ifdef _WIN32
    Beep(400, 500);  // Lower pitch, longer duration  
#else
    // Multiple beeps for error
    cout << "\a" << flush;
    cout << "\a" << flush;
#endif
}

//EFFECTS Plays sound for warnings (warning beep)
void play_warning_sound() {
#ifdef _WIN32
    Beep(600, 300);  // Mid pitch, medium duration
#else
    cout << "\a" << flush;
#endif  
}

//EFFECTS Plays sound for game events (generic beep)
void play_game_sound() {
#ifdef _WIN32
    Beep(1000, 150);  // High pitch, very short
#else
    cout << "\a" << flush;
#endif
}