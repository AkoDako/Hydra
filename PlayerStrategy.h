#ifndef _PLAYER_STRATEGY_H_
#define _PLAYER_STRATEGY_H_

#include "Card.h"

class Game;
class Player;


class PlayerStrategy {
    public:
    virtual Value readInValue(bool testingEnabled);
    virtual Suit readInSuit();
    //take action returns true if a card was played or the player's turn ended, and false otherwise
    //(e.g. would return false if player reserved, got invalid input, swapped, etc.)
    virtual bool takeAction(Player & player, Game & game) = 0;
    virtual ~PlayerStrategy();
};

#endif
