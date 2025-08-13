#ifndef SHOP_HPP
#define SHOP_HPP

#include <vector>
#include <memory>
#include <string>
#include "Joker.hpp"

struct ShopItem {
    std::string name;
    int cost;
    JokerRarity rarity;
    std::string description;
    
    //EFFECTS Constructs a shop item
    ShopItem(const std::string& name, int cost, JokerRarity rarity, const std::string& desc)
        : name(name), cost(cost), rarity(rarity), description(desc) {}
};

class Shop {
public:
    //EFFECTS Initializes shop with empty inventory
    Shop();
    
    //EFFECTS Destructor cleans up shop
    ~Shop();
    
    //EFFECTS Restocks shop with random jokers
    void restock();
    
    //EFFECTS Returns current shop inventory
    const std::vector<ShopItem>& get_inventory() const;
    
    //EFFECTS Returns true if item at index can be bought with given money
    //REQUIRES index is valid
    bool can_afford(int index, int money) const;
    
    //EFFECTS Purchases item at index, returns joker pointer
    //REQUIRES index is valid and player can afford item
    //MODIFIES Removes item from inventory
    std::unique_ptr<Joker> buy_item(int index);
    
    //EFFECTS Returns cost to restock shop
    int get_restock_cost() const;
    
    //EFFECTS Increases restock cost (gets more expensive each time)
    void increase_restock_cost();
    
    //EFFECTS Returns number of items in shop
    size_t get_item_count() const;
    
    //EFFECTS Clears all items from shop
    void clear_inventory();

private:
    std::vector<ShopItem> inventory;
    int restock_cost;
    
    //EFFECTS Generates a random joker for the shop
    ShopItem generate_random_joker() const;
    
    //EFFECTS Returns list of all available joker names
    std::vector<std::string> get_joker_pool() const;
    
    //EFFECTS Returns random rarity based on weighted probabilities
    JokerRarity get_random_rarity() const;
};

//EFFECTS Prints ShopItem to stream
std::ostream& operator<<(std::ostream& os, const ShopItem& item);

#endif // SHOP_HPP