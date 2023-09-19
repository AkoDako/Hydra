#include "PlayerStrategy.h"
#include "Card.h"
#include "Game.h"
#include "Player.h"


#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

PlayerStrategy::~PlayerStrategy() {}

Value PlayerStrategy::readInValue(bool testingMode) {
    string input;
    if(!(cin >> input)) {
        throw runtime_error("Input could not be read in.");
    }

    //check for royals
    if (input == "K") {
        return Value::King;
    }
    else if (input == "Q") {
        return Value::Queen;
    }
    else if (input == "J") {
        return Value::Jack;
    }
    else if (input == "A") {
        return Value::Ace;
    }
    else if (input == "Joker") {
        if (!testingMode) {
            throw invalid_argument("Setting a card's value to Joker outside of testing mode is not valid");
        }
        else {
            return Value::Joker;
        }
    }

    //see if the input was an integer between 2 and 10 inclusive
    else {
        try {
            int value = stoi(input);
            if (!(value <= 10 && value >= 2)) {
                throw invalid_argument("Input was not a valid card value");
            }
            else {
                //got a valid card value so we set the joker to the value 
                return static_cast<Value>(value);
            }
        }
        catch (invalid_argument & e) {
            throw invalid_argument("Input was not a valid card value");
        }
    }
    //we throw before we return this so it doesnt really matter
    return Value::Jack;
}

Suit PlayerStrategy::readInSuit() {
    char input;
    if(!(cin >> input)) {
        throw runtime_error("Input could not be read in");
    }

    switch (input) {
        case 'S': 
            return Suit::Spades;
            break;
        case 'D':
            return Suit::Diamonds;
            break;
        case 'H':
            return Suit::Hearts;
            break;
        case 'C':
            return Suit::Clubs;
            break;
        default:
            throw invalid_argument("Valid suit was not read in.");
    }
    //doesnt really matter what return since we throw or return before we get here
    return Suit::Spades;
}
