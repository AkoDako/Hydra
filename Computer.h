#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "PlayerStrategy.h"

class Game;
class Player;

//Computer player strategy
//Computer players are only enabled if enhancements are enabled 
//Therefore they also have all other enhancements enabled in their behaviour
class Computer: public PlayerStrategy {
    public:
    virtual bool takeAction(Player & player, Game & game) override;
};

#endif
