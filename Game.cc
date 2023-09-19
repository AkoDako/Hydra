#include "Game.h"
#include "Player.h"
#include "Card.h"
#include "Pile.h"
#include "Computer.h"
#include "SuitValue.h"

#include <vector>
#include <memory>
#include <iostream>

using namespace std;

Game::Game(int numberOfPlayers, bool testingEnabled, bool enhancementsEnabled, int drawPileSize): youngestHead{0}, testingEnabled{testingEnabled},
    enhancementsEnabled{enhancementsEnabled} {
    
    for (int i = 0; i < numberOfPlayers; i++) {
        players.emplace_back(Player(i+1));
    }

    //create as many decks as there are players and move them into 1 big pile
    Pile bigPile;
    for (int i = 0; i < numberOfPlayers; i++) {
        //create temporary pile, set its contents to a deck and then move it onto the big pile
        Pile temp;
        temp.setDeck();
        temp.movePile(bigPile, false);
    } 
    bigPile.shufflePile();
    //distribute cards
    for (int i = 0; i < drawPileSize; i++) {
        for (auto &p: players) {
            p.addCardToDraw(bigPile);
        }
    }
}

bool Game::processRound() {
    for (auto & p: players) {
        try {
            if (p.takePlayerTurn(*this)) {
                return true;
            }
        }
        catch (runtime_error & e) {  
            cout << e.what() << endl;
            throw runtime_error("Terminating program, reached end of input");
        }
    }
    return false;
}

int Game::getYoungestHead() const {
    return youngestHead;
}

int Game::getLiveHeads() const {
    //count number of live (i.e. non empty piles) heads
    int counter = 0;
    for (auto &h: heads) {
        if (!h.isEmpty()) {
            counter++;
        }
    }
    return counter;
}


int Game::getOldestHead() const {
    for (int i = 0; i < youngestHead; i++) {
        if (heads[i].getSize() > 0) {
            return i+1;
        }
    }
    return 1;
}

int Game::getPlayerCount() const {
    return players.size();
}

bool Game::isTestingEnabled() const {
    return testingEnabled;
}

bool Game::isHeadAlive(int n) const {
    return !(heads[n-1].isEmpty());
}

Value Game::getHeadValue(int n) const {
    return heads[n-1].peekTopCard().getValue();
}

Suit Game::getHeadSuit(int n) const {
    return heads[n-1].peekTopCard().getSuit();
}

const Card & Game::getHeadCard(int n) const {
    return heads[n-1].peekTopCard();
}

int Game::getHeadSize(int n) const {
    return heads[n-1].getSize();
}

void Game::placeOnHead(int playerNumber, int headNumber) {
    players[playerNumber-1].playHand(heads[headNumber-1], true);
}

void Game::cutHead(int playerNumber) {
    int i = getOldestHead();
    Pile empty;
    //move head to player's discard pile
    players[playerNumber-1].moveToDiscard(heads[i-1]);
    //make 2 new empty heads
    for (int i = 0; i < 2; i++) {
        heads.emplace_back(empty);
        youngestHead++;
    }
    return;
    //PlayerStrategy method handles the rest
}

void Game::printPlayer(int playerNumber) const {
    int drawSize = players[playerNumber-1].drawPileSize();
    int discardSize = players[playerNumber-1].discardPileSize();
    cout << "Player " << playerNumber << ": " << drawSize + discardSize << " (" << drawSize << " draw, " << discardSize << " discard)";
}

void Game::printHeads() const {
    int n = getYoungestHead();
    cout << "Heads: " << endl;
    for (int i = 0; i < n; i++) {
        if (isHeadAlive(i+1)) {
            cout << i+1 << " : " << heads[i].peekTopCard() << " (" << getHeadSize(i+1) << ")" << endl;
        }
    }
}

void Game::init() {
    //uses player 1's hand to make the first head
    Pile p;
    heads.emplace_back(p);
    placeOnHead(1, 1);
    youngestHead = 1;
}

void Game::toggleComputer(int playerNumber) {
    if (!enhancementsEnabled) {
        cout << "Can't toggle player control: computers disabled" << endl;
    }
    else {
        auto strat = make_shared<Computer>();
        players[playerNumber-1].setStrategy(strat);
    }
}

bool Game::areComputersEnabled() const {
    return enhancementsEnabled;
}


