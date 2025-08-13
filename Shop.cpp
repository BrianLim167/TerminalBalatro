#include "Shop.hpp"
#include "ImprovedShuffle.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

//EFFECTS Initializes shop with empty inventory
Shop::Shop() : restock_cost(2) {
    restock();
}

//EFFECTS Destructor cleans up shop
Shop::~Shop() {
    // vector automatically cleans up
}

//EFFECTS Restocks shop with random jokers
void Shop::restock() {
    clear_inventory();
    
    // Generate 2-4 random jokers for shop
    auto& gen = ImprovedShuffle::get_rng();
    uniform_int_distribution<> item_count(2, 4);
    
    int num_items = item_count(gen);
    for (int i = 0; i < num_items; ++i) {
        inventory.push_back(generate_random_joker());
    }
}

//EFFECTS Returns current shop inventory
const vector<ShopItem>& Shop::get_inventory() const {
    return inventory;
}

//EFFECTS Returns true if item at index can be bought with given money
//REQUIRES index is valid
bool Shop::can_afford(int index, int money) const {
    assert(index >= 0 && index < static_cast<int>(inventory.size()));
    return money >= inventory[index].cost;
}

//EFFECTS Purchases item at index, returns joker pointer
//REQUIRES index is valid and player can afford item
//MODIFIES Removes item from inventory
unique_ptr<Joker> Shop::buy_item(int index) {
    assert(index >= 0 && index < static_cast<int>(inventory.size()));
    
    ShopItem item = inventory[index];
    inventory.erase(inventory.begin() + index);
    
    return unique_ptr<Joker>(Joker_factory(item.name));
}

//EFFECTS Returns cost to restock shop
int Shop::get_restock_cost() const {
    return restock_cost;
}

//EFFECTS Increases restock cost (gets more expensive each time)
void Shop::increase_restock_cost() {
    restock_cost += 2; // Increase by $2 each time
}

//EFFECTS Returns number of items in shop
size_t Shop::get_item_count() const {
    return inventory.size();
}

//EFFECTS Clears all items from shop
void Shop::clear_inventory() {
    inventory.clear();
}

//EFFECTS Generates a random joker for the shop
ShopItem Shop::generate_random_joker() const {
    vector<string> joker_pool = get_joker_pool();
    
    auto& gen = ImprovedShuffle::get_rng();
    uniform_int_distribution<> joker_dist(0, joker_pool.size() - 1);
    
    string joker_name = joker_pool[joker_dist(gen)];
    
    // Create temporary joker to get properties
    unique_ptr<Joker> temp_joker(Joker_factory(joker_name));
    
    int cost = temp_joker->get_cost();
    JokerRarity rarity = temp_joker->get_rarity();
    string description = temp_joker->get_description();
    
    // Apply random price variation (+/-1)
    uniform_int_distribution<> price_var(-1, 1);
    cost = max(1, cost + price_var(gen));
    
    return ShopItem(joker_name, cost, rarity, description);
}

//EFFECTS Returns list of all available joker names
vector<string> Shop::get_joker_pool() const {
    return {
        "Joker",
        "Pair Joker", 
        "Flush Joker",
        "Face Card Joker"
    };
}

//EFFECTS Returns random rarity based on weighted probabilities
JokerRarity Shop::get_random_rarity() const {
    auto& gen = ImprovedShuffle::get_rng();
    uniform_int_distribution<> rarity_roll(1, 100);
    
    int roll = rarity_roll(gen);
    
    // Weighted probabilities: Common 60%, Uncommon 30%, Rare 8%, Legendary 2%
    if (roll <= 60) {
        return COMMON;
    } else if (roll <= 90) {
        return UNCOMMON;
    } else if (roll <= 98) {
        return RARE;
    } else {
        return LEGENDARY;
    }
}

//EFFECTS Prints ShopItem to stream
ostream& operator<<(ostream& os, const ShopItem& item) {
    os << item.name << " ($" << item.cost << ") - " << item.rarity << ": " << item.description;
    return os;
}