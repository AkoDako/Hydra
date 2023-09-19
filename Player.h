#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Pile.h"
#include "SuitValue.h"
#include <memory>
#include <iostream>
#include <string>

class PlayerStrategy;
class Game;

class Player {
    
    int playerNumber;
    std::shared_ptr<PlayerStrategy> strat;
    bool endedTurn = false;

    Pile drawPile;
    Pile discardPile;
    Pile hand;
    Pile reserve;

    //while both a player's hand and reserve can only have at max one card, which would
    //maybe suggest that we should store them as a card, they can also be empty at times, so 
    //I decided to use a pile to store them.

    public:
    Player(int playerNumber);
    ~Player();
    //acccessors
    
    //returns reference to the card in player's hand
    Card & getHand();

    int getNumber() const;
    int drawPileSize() const;
    int discardPileSize() const;

    //while technically the reserve and handcan only hold one card at maximum under standard rules, 
    //which might suggest that we have these return boolean values for empty vs not empty,
    //house rules may change this, so we have them return the size
    int reserveSize() const;
    int handSize() const;

    //Player Operations:
    //methods assume caller has validated the move/input (caller is usually the strategy)

    //sets card in hand to specified value and suit (for testing mode)
    void setHand(Value val, Suit suit);

    //changes what strategy the player uses (e.g. what strat points to)
    //if the player is controlled by the computer it switches to human control and vice versa
    void setStrategy(std::shared_ptr<PlayerStrategy> ps);

    //moves top card of draw pile into player's hand
    //if draw pile is empty shuffles discard pile into draw pile then draws a card
    void drawCard();  

    //swaps card in player's hand with the one in reserve 
    //if reserve is empty draws another card
    //assumes the player's draw pile and discard pile aren't empty
    void placeInReserve();

    //moves all cards from pile p face down into the player's discard pile
    void moveToDiscard(Pile & p);

    //places card in player's hand onto specified pile face up or face down as specified
    void playHand(Pile & p, bool isFaceUp);

    //moves card(s) in player's hand to discard pile
    //requires: player's hand is not empty
    void discardHand();

    //takes in a pile by reference and adds the top card to the player's draw pile
    //used for setting up game
    void addCardToDraw(Pile & p);

    //moves player's reserve pile to their discard pile
    //used at the end of the player's turn if they have any cards left in reserve
    //requires: there are cards in the player's reserve
    //caller responsible for checking if there are cards in players reserve
    void clearReserve();

    //returns true if player's draw pile, reserve and discard are all empty
    //only called after a player takes an action
    bool hasWon() const;
    
    //invokes strategy to determine how to take turn
    //returns true if the player won, otherwise returns false
    bool takePlayerTurn(Game & game);

    //outputs to standard output representing player's hand with proper grammar
    //requires: player has a card in hand
    //grammar enhancement
    void outputHand() const;

    //sets player's endedTurn bool to true
    void endTurn();
    
};

std::ostream & operator<<(std::ostream & out, Player & p);

#endif
