#ifndef _GAME_H_
#define _GAME_H_

#include "SuitValue.h"

#include <vector>
#include <map>
#include <memory>
#include <iostream>

class Card;
class Pile;
class Player;

class Game {
    //youngestHead stores the total number of heads that have existed in the game so far, i.e. the number of the youngest head
    int  youngestHead;
    bool testingEnabled;
    bool enhancementsEnabled;

    std::vector<Pile> heads;
    std::vector<Player> players;

    
    public:
    Game(int numberOfPlayers, bool testingEnabled, bool enhancementsEnabled, int drawPileSize);

    //returns true if a player won
    bool processRound();

    //requires: there is at least one live (non empty) head
    int getYoungestHead() const;
    //returns number of oldest head
    //requires: there is at least one live (non empty) head
    int getOldestHead() const;
    int getPlayerCount() const;
    bool isTestingEnabled() const;
    
    //returns true if a head is alive (i.e. not empty) and false if it isn't
    //requires: n is the number of a valid head
    bool isHeadAlive(int n) const;

    //returns the number of heads that are alive
    int getLiveHeads() const;

    //returns the value of head n, i.e. the value of the top card of head at heads[n]
    //requires: n to be the number of a valid head (i.e one that has not been cut off)
    Value getHeadValue(int n) const;
    
    //returns the suit of head n, i.e. the suit of the top card of head at heads[n]
    //requires: n to be the number of a valid head (i.e one that has not been cut off)
    Suit getHeadSuit(int n) const;

    const Card & getHeadCard(int n) const;    
    int getHeadSize(int n) const;

    //Game Operations:

    //none of these methods have exception safety built in.  Concrete PlayerStrategy subclasses will
    //call these in their methods and are responsible for ensuring that calling these methods will
    //not cause an error (ie are responsible for checking whether moves are valid or not)

    //takes in the number of a player and the number of a head, and places the card
    //from the player's hand onto head with the specified number
    //requires: player with the given number has a card in hand that can be played on the specified head
    void placeOnHead(int playerNumber, int headNumber);

    //Takes in the number of a player and makes that player cut off a head
    //requires: it is the player's turn
    void cutHead(int playerNumber);

    //outputs player's info in the format:
    //Player (playerNumber): (draw pile size + discard pile size) ( (draw pile size) draw, (discard pile size) discard)
    //doesnt input turn specific information (e.g. hand size, remaining cards to be played)
    void printPlayer(int playerNumber) const;

    //prints out all heads in format specified in design document
    void printHeads() const;

    //if player is human gives control to computer
    //requires: player with specified number exists
    //if enhancements aren't enabled does nothing
    void toggleComputer(int playerNumber);

    void init();

    bool areComputersEnabled() const;

};

#endif
