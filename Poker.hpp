#ifndef POKER_HPP
#define POKER_HPP

#include "Card.hpp"
#include <vector>
#include <string>

using namespace std;


enum HandType {
    //poker hands
    HIGH_CARD = 1,
    PAIR = 2,
    TWO_PAIR = 3,
    THREE_OF_KIND = 4,
    STRAIGHT = 5,
    FLUSH = 6,
    FULL_HOUSE = 7,
    FOUR_OF_KIND = 8,
    STRAIGHT_FLUSH = 9,
    ROYAL_FLUSH = 10,

    //balatro hands
    FIVE_OF_KIND = 11,
    FLUSH_HOUSE = 12, //fullHouse + flush
    FLUSH_FIVE = 13 //fiveOfKind + flush

};

class PokerHand {
    public:
    //constructors
    PokerHand();
    PokerHand(const vector<Card>& cards);

    HandType evaluate_hand();

    int get_base_chips() const;
    int get_base_mult() const;
    int calculate_score() const;

    //get hand type
    HandType get_type() const;

    //set cards
    void set_cards(const vector<Card> & cards);

    //add card
    void add_card(const Card& card);

    //clear hand
    void clear(); 

    //hand size
    size_t size() const;

    private:
        vector<Card> cards;
        HandType type;
        int base_chips;
        int base_mult;

        //check hands
        bool is_flush() const;
        bool is_highCard() const;
        bool is_pair() const;
        bool is_twoPair() const;
        bool is_threeOfKind() const;
        bool is_straight() const;
        bool is_fullHouse() const;
        bool is_fourOfKind() const;
        bool is_straightFlush() const;
        bool is_royalFlush() const;

        bool is_fiveOfKind() const;
        bool is_flushHouse() const;
        bool is_flushFive() const;

        vector<int> get_rank_counts() const;

        void calculate_base_values(); //set base chips/mult based on hand type
};

string hand_type_to_string(HandType type);

#endif //poker.hpp
