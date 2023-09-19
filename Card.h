#ifndef _CARD_H_
#define _CARD_H_

#include "SuitValue.h"
#include <iostream>

class Card {
    Suit suit;
    Value value;
    bool faceUp;

    public:
    Card(Suit s, Value v, bool faceUp);
    Card();
    void setValue(Value val);
    Value getValue() const;
    Suit getSuit() const;
    void flipCard();
    bool isFaceUp() const;
};

//card output operator employs correct grammar
//if enhancements aren't on we just output the value and suit of a card individually
std::ostream & operator<<(std::ostream & out, Card c);

#endif
