#include "Player.h"
#include "Pile.h"
#include "Card.h"
#include "PlayerStrategy.h"
#include "Human.h"
#include "Game.h"
#include <memory>
#include <vector>
#include <iostream>

using namespace std;

Player::Player(int playerNumber): playerNumber(playerNumber), strat{make_shared<Human>()} {}

Player::~Player() {}

Card & Player::getHand() {
    if (hand.getSize() != 1) {
        throw logic_error("Hand isn't of size 1");
    }
    return hand.getTopCard();
}

int Player::drawPileSize() const {
    return drawPile.getSize();
}

int Player::discardPileSize() const {
    return discardPile.getSize();
}

int Player::reserveSize() const {
    return reserve.getSize();
}

int Player::handSize() const {
    return hand.getSize();
}

int Player::getNumber() const {
    return playerNumber;
}

//invokes strategy to determine how to take actions
//returns true if the player won, otherwise returns false
bool Player::takePlayerTurn(Game & game) {
    //live heads = number of heads that currently exist
    //largest head = number of the largest head that currently exists
    int liveHeads = game.getLiveHeads();
    int playerCount = game.getPlayerCount();
    int cardsPlayed = 0;

    //print out game state unless its the first turn (i.e. theres 0 heads alive)
    if (liveHeads == 0) {}
    else {
        game.printHeads();
        cout << endl;
        cout << "Players: " << endl;
        for (int j = 0; j < playerCount; j++) {
            game.printPlayer(j+1);
            cout << endl;
        }
        cout << endl;
        cout << "Player " << playerNumber << ", it is your turn." << endl;
        cout << endl;
    }

    do {
        drawCard();
        //invoke strategy to determine how to take turn
        //take actions until we play a card or end our turn, at which point we exit while(true) loop
        //takeAction returns true if the player played a card or ended their turn and false otherwise
        endedTurn = false;
        while (true) {
            //output gamestate unless its the first turn 
            if (!(liveHeads == 0)) {
                game.printHeads();
                cout << endl;
                cout << "Players: " << endl;
                for (int j = 0; j < playerCount; j++) {
                    game.printPlayer(j+1);
                    //if we're printing the player whose turn it is currently we also print their hand size, reserve size
                    //and the number of cards they still need to play in the turn
                    if (j+1 == playerNumber) {
                        cout << " + " << handSize() << " in hand, " << liveHeads - cardsPlayed << " remaining, " << reserveSize() << " in reserve";
                    }
                    cout << endl;
                }
            }

            if (strat->takeAction(*this, game)) {//if the player played a card or ended their turn
                if (hasWon()) {
                    cout << "Player " << playerNumber << " wins!" << endl;
                    return true;
                }
                break;
            }
        }
        //if the player ended their turn in takeAction, we set the flag back to false and end the player's turn
        //by breaking out of the do/while loop
        if (endedTurn) {
            endedTurn = false;
            break;
        }
        cardsPlayed++;
    }
    while (cardsPlayed < liveHeads);

    //if the player's reserve is not empty after their turn has ended they move it to their discard
    if (reserveSize() != 0) {
        clearReserve();
    }
    if (hasWon()) {
        cout << "Player " << playerNumber << " wins!" << endl;
        return true;
    }
    return false;
}

bool Player::hasWon() const {
    return (drawPileSize() == 0 && discardPileSize() == 0 && handSize() == 0 && reserveSize() == 0);
}

void Player::drawCard() {
    if (drawPile.getSize() == 0) {//draw pile is empty so we shuffle our discard pile in
        discardPile.movePile(drawPile, false);
        drawPile.shufflePile();
    }
    drawPile.moveTopCard(hand, true);
}

void Player::placeInReserve() {
    if (reserve.getSize() == 1) {
        Pile temp;
        hand.moveTopCard(temp, false);
        reserve.moveTopCard(hand, true);
        temp.moveTopCard(reserve, false);
    }
    else {
        hand.moveTopCard(reserve, false);
    }
    //doesn't draw an extra card if the player didn't have a card in reserve; the strategy does that
}

//moves all cards from pile p face down into the player's discard pile
//usually for moving heads in
void Player::moveToDiscard(Pile & p) {
    p.movePile(discardPile, false);
}

void Player::setHand(Value val, Suit suit) {
    //moves the current hand to the trash then adds the new card to the hand
    Pile trash;
    hand.moveTopCard(trash, true);
    Card newCard = Card(suit, val, true);
    hand.addCard(newCard, true);
}

void Player::setStrategy(std::shared_ptr<PlayerStrategy> ps) {
    strat = ps;
}

void Player::playHand(Pile & p, bool isFaceUp) {
    //move top (and only) card in player's hand pile to the specified location
    hand.moveTopCard(p, isFaceUp);
}

void Player::discardHand() {
    hand.movePile(discardPile, false);
}

void Player::addCardToDraw(Pile & p) {
    p.moveTopCard(drawPile, false);
}

void Player::clearReserve() {
    reserve.movePile(discardPile, false);
}

void Player::endTurn() {
    endedTurn = true;
}
