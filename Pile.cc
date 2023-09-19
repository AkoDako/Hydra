#include "Pile.h"
#include "Card.h"
#include "SuitValue.h"

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <random>
#include <stdexcept>
#include <iostream>

using namespace std;

Pile::Pile() {
    pile.resize(0);
}

Pile::~Pile() {}

int Pile::getSize() const {
    return pile.size();
}

void Pile::shufflePile() {
    srand (unsigned ( time(0) ));
    random_shuffle(pile.begin(), pile.end());
}

Card & Pile::getTopCard() {
    if (pile.size() == 0) {throw range_error("Pile is empty");}
    return pile.back();
}

const Card & Pile::peekTopCard() const {
    if (pile.size() == 0) {throw range_error("Pile is empty");}
    return pile.back();
}

void Pile::addCard(Card c, bool faceUp) {
    pile.push_back(c);
    if (pile.back().isFaceUp() == faceUp) {}
    else {pile.back().flipCard();}
}

void Pile::moveTopCard(Pile & p, bool faceUp) {
    if (pile.size() == 0) {throw range_error("Pile is empty");}
    
    p.addCard(pile.back(), faceUp);
    pile.pop_back();    
}

void Pile::movePile(Pile & p, bool faceUp) {
    for (auto c: this->pile) {
        p.addCard(c, faceUp);
    }
    this->pile.clear();
}

bool Pile::isEmpty() const {
    return pile.size() == 0;
}

void Pile::setDeck() {
    pile.clear();
    //add 1 card of every suit/value
    for (int i = 1; i < 14; i++) {
        pile.emplace_back(Card(Suit::Clubs, static_cast<Value>(i), false));
        pile.emplace_back(Card(Suit::Hearts, static_cast<Value>(i), false));
        pile.emplace_back(Card(Suit::Diamonds, static_cast<Value>(i), false));
        pile.emplace_back(Card(Suit::Spades, static_cast<Value>(i), false));
    }

    //add jokers
    pile.emplace_back(Card(Suit::Joker, Value::Joker, false));
    pile.emplace_back(Card(Suit::Joker, Value::Joker, false));
}

