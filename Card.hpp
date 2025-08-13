#ifndef CARD_HPP
#define CARD_HPP
#include <iostream>

enum Rank {
  TWO     = 0,
  THREE   = 1,
  FOUR    = 2,
  FIVE    = 3,
  SIX     = 4,
  SEVEN   = 5,
  EIGHT   = 6,
  NINE    = 7,
  TEN     = 8,
  JACK    = 9,
  QUEEN   = 10,
  KING    = 11,
  ACE     = 12,
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str);

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank);

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank);

// Represent a Card's suit
enum Suit {
  SPADES   = 0,
  HEARTS   = 1,
  CLUBS    = 2,
  DIAMONDS = 3,
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str); //maybe could be removed?

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit);

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit);


class Card {
public:

  //EFFECTS Initializes Card to the Two of Spades
  Card();

  //EFFECTS Initializes Card to specified rank and suit
  Card(Rank rank_in, Suit suit_in);

  //EFFECTS Returns the rank
  Rank get_rank() const;

  //EFFECTS Returns the suit
  Suit get_suit() const;


  //EFFECTS Returns true if card is a face card (Jack, Queen, King or Ace)
  bool is_face_or_ace() const;


private:
  Rank rank;
  Suit suit;

  // This "friend declaration" allows the implementation of operator>>
  // to access private member variables of the Card class.
  friend std::istream & operator>>(std::istream &is, Card &card);
};

//EFFECTS Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card);

//EFFECTS Reads a Card from a stream in the format "Two of Spades"
//NOTE The Card class declares this operator>> "friend" function,
//     which means it is allowed to access card.rank and card.suit.
std::istream & operator>>(std::istream &is, Card &card);

//EFFECTS Returns true if lhs is lower value than rhs
bool operator<(const Card &lhs, const Card &rhs);

//EFFECTS Returns true if lhs is lower value than rhs or the same card as rhs
bool operator<=(const Card &lhs, const Card &rhs);

//EFFECTS Returns true if lhs is higher value than rhs
bool operator>(const Card &lhs, const Card &rhs);

//EFFECTS Returns true if lhs is higher value than rhs or the same card as rhs
bool operator>=(const Card &lhs, const Card &rhs);

//EFFECTS Returns true if lhs is same card as rhs
bool operator==(const Card &lhs, const Card &rhs);

//EFFECTS Returns true if lhs is not the same card as rhs
bool operator!=(const Card &lhs, const Card &rhs);


#endif // CARD_HPP
