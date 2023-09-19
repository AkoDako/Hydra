#include "Card.h"
#include "SuitValue.h"
#include <stdexcept>
#include <exception>
#include <iostream>

using namespace std;

Card::Card(Suit s, Value v, bool faceUp): suit{s}, value{v}, faceUp{faceUp} {}
Card::Card() {}


void Card::setValue(Value val) {
    this->value = val;
}

Value Card::getValue() const {
    return value;
}

Suit Card::getSuit() const {
    return suit;
}

void Card::flipCard() {
    if (faceUp) faceUp = false;
    else { faceUp = true;}
}

bool Card::isFaceUp() const {
    return faceUp;
}

ostream & operator<<(ostream & out, const Card c) {
    Value v = c.getValue();
    out << v;
    out << static_cast<char>(c.getSuit());
    return out;
}

