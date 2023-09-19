#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "PlayerStrategy.h"

class Human: public PlayerStrategy {
    public:
    virtual bool takeAction(Player & player, Game & game) override;
};

#endif
