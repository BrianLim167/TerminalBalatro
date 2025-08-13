#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>
#include "Card.hpp"

//EFFECTS Clears the console screen
void clear_screen();

//EFFECTS Waits for user to press Enter
void wait_for_enter();

//EFFECTS Prints a horizontal line separator
void print_separator();

//EFFECTS Prints centered text with padding
void print_centered(const std::string& text, int width = 50);

//EFFECTS Gets integer input from user with validation
//REQUIRES min <= max
int get_int_input(const std::string& prompt, int min, int max);

//EFFECTS Gets yes/no input from user, returns true for yes
bool get_yes_no_input(const std::string& prompt);

//EFFECTS Gets multiple integer inputs (for selecting cards)
//REQUIRES max_selections > 0 and available_count > 0
std::vector<int> get_multiple_int_input(const std::string& prompt, 
                                       int max_selections, 
                                       int available_count);

//EFFECTS Prints a vector of cards with indices
void print_cards_with_indices(const std::vector<Card>& cards);

//EFFECTS Prints a vector of cards as ASCII art in horizontal layout
void print_ascii_cards_horizontal(const std::vector<Card>& cards);

//EFFECTS Returns ASCII art representation of a single card as vector of strings
std::vector<std::string> get_ascii_card(const Card& card);

//EFFECTS Prints game stats in a formatted way
void print_game_stats(int ante, const std::string& blind_name, 
                     int score, int needed, int hands, int discards, int money);

//EFFECTS Formats large numbers with commas (e.g., 1000 -> "1,000")
std::string format_number(int number);

//EFFECTS Returns a string with specified number of spaces
std::string spaces(int count);

//EFFECTS Validates that indices are in valid range
//REQUIRES indices is not empty
bool validate_indices(const std::vector<int>& indices, int max_valid);

//EFFECTS Removes duplicates from vector and sorts it
std::vector<int> clean_indices(std::vector<int> indices);

//EFFECTS Prints error message in red (if terminal supports colors)
void print_error(const std::string& message);

//EFFECTS Prints success message in green (if terminal supports colors)
void print_success(const std::string& message);

//EFFECTS Prints warning message in yellow (if terminal supports colors)
void print_warning(const std::string& message);

//EFFECTS Returns random integer between min and max (inclusive)
int random_int(int min, int max);

//EFFECTS Seeds random number generator with current time
void seed_random();

//EFFECTS Plays sound for successful actions (success beep)
void play_success_sound();

//EFFECTS Plays sound for errors or failures (error beep)
void play_error_sound();

//EFFECTS Plays sound for warnings (warning beep) 
void play_warning_sound();

//EFFECTS Plays sound for game events (generic beep)
void play_game_sound();

#endif // UTILS_HPP