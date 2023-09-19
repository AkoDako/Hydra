#ifndef _SUIT_VALUE_H_
#define _SUIT_VALUE_H_

#include <iostream>

enum class Suit: char { Clubs = 'C', Hearts = 'H', Spades = 'S', Diamonds = 'D', Joker = 'J' };
enum class Value: int { Ace = 1, Two = 2, Three = 3, Four = 4, Five = 5, Six = 6, Seven = 7, Eight = 8, Nine = 9, Ten = 10, Jack = 11, Queen = 12, King = 13, Joker = 14};

std::ostream & operator<<(std::ostream & out, Suit & suit);
std::ostream & operator<<(std::ostream & out, Value & value);

#endif
