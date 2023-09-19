#include "Computer.h"
#include "Game.h"
#include "Player.h"
#include "Card.h"
#include "Pile.h"
#include "SuitValue.h"
#include "PlayerStrategy.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;


//value of jokers used to make new heads
//can be changed if rules change
const Value newHeadJokerVal = Value::Two;

bool Computer::takeAction(Player & player, Game & game) {
    int liveHeads = game.getLiveHeads();
    int playerNum = player.getNumber();
    int oldestHead = game.getOldestHead();
    int youngestHead = game.getYoungestHead();
    bool testingEnabled = game.isTestingEnabled(); 
    bool reserveCardPlayable = false;
    bool hasPlayedCard = false;

    //if a player's only remaining card is in reserve they put it in their discard pile and end their turn
    //ends turn by returning false (we ended our turn so we return false)
    if (player.drawPileSize() == 0 && player.reserveSize() == 1 && player.discardPileSize() == 0 && player.handSize() == 0) {
        player.clearReserve();
        player.endTurn();
        return true;
    }

    //loop until the player takes a valid action in the loop
    while (!hasPlayedCard) {
    //if a player's hand is empty (because they reserved) we check if they have another card to draw
    //if their only card is inreserve they move that card to discard and end their turn
    //end player's turn and return true 
       

        //if game is in testing mode we prompt user for input until we get a valid card
        if (testingEnabled) {
            //read in input until the player's hand is set to a valid card
            while (true) {
                cout << "Card value?" << endl;
                try {
                    Value val = readInValue(testingEnabled);
                    Suit suit;
                    if (val == Value::Joker) {
                        suit = Suit::Joker;
                    }
                    else {
                        cout << "Card suit?" << endl;
                        suit = readInSuit();
                    }
                    player.setHand(val, suit);
                    break;
                    //successfully set player's hand to a valid card
                }
                catch (invalid_argument & i) {
                    cout << i.what() << endl << endl;
                }
                catch (runtime_error & r) {
                    cout << r.what() << endl << endl;
                    throw runtime_error("Input could not be read in, ending program.");
                }
            }
        }
            
        //if theres no heads the computer creates the first head by playing their hand and immediately ends their turn
        //if the computer has a joker it sets its value to 2 to make the next player's turn harder
        if (liveHeads == 0) {
            if (player.getHand().getSuit() == Suit::Joker) {
                player.getHand().setValue(Value::Two);
            }
            game.init();
            //ends turn so we return true
            player.endTurn();
            return true;
        }

        Card hand = player.getHand();
        Suit handSuit = hand.getSuit();
        Value handVal = hand.getValue();
        cout << endl;
        cout << "(Computer) Player " << playerNum << " is holding a " << hand << endl;

        //if the computer has a joker in hand and the card in reserve is playable it saves the joker for later
        if (handSuit == Suit::Joker && reserveCardPlayable) {
            cout << "(Computer) Player " << playerNum <<  " placed a" << hand << " in reserve." << endl;
            player.placeInReserve();
            reserveCardPlayable = true;
            //take another action, return false
            return false;;
        }

        //computer checks for valid moves first
        bool hasValidMove = false;
        int bestMove = 0;
        Value lowestHeadVal = Value::King;

        if (handSuit == Suit::Joker) {
            hasValidMove = true;
            bestMove = oldestHead;
        }

        //computer tries to store the number of the head with the lowest head value that is greater than its current hand value
        //i.e. the number of a head which the computer can play its hand on and not have its turn end immediately
        //e.g. if the computer is holding a 5, and the head 1 has a 5 and head 2 has a 6, bestMove will store 2
        //if no such head exists it stores any possible move (including ones for which playing its hand would end their turn immediately)         
        
        //computer considers the "best" move to be the one that gives it the most future flexibility
        //i.e. the one that gives it the highest chance of playing another card next turn and avoiding cutting off a head
        
        //check every head for moves, starting at the youngest one
        for (int i = oldestHead; i < youngestHead+1; i++) {
            Value headVal = game.getHeadValue(i);
            if (handVal <= headVal || headVal == Value::Ace) {
                //if we haven't stored a move yet
                if (bestMove == 0) {
                    bestMove = i;
                    lowestHeadVal = headVal;
                }
                hasValidMove = true;
            }
            //if the current head's value is lower than the lowest head value so far 
            //but can still be played on without ending the computer's turn immediately
            //it store's that heads number
            if (handVal < headVal && headVal < lowestHeadVal) {
                bestMove = i;
            }
        }
        //if computer cannot play the card in hand:
        if (!hasValidMove) {
            int reserveSize = player.reserveSize();
            int drawPileSize = player.drawPileSize();
            int discardPileSize = player.discardPileSize();
            //place hand in reserve if the reserve pile is empty or if the reserve card is playable
            //only does so if the computer also has another card to draw
            if ((reserveSize == 0 || reserveCardPlayable) && (drawPileSize != 0 && discardPileSize != 0)) {
                cout << "(Computer) Player " << playerNum <<  " placed a ";
                if (hand.getSuit() == Suit::Joker) {
                    cout << "Joker" << endl;
                }
                else {
                    cout << hand << " in reserve" << endl;
                } 
                player.placeInReserve();
                reserveCardPlayable = false;
                player.drawCard();
                //player takes another action
                return false;
            }
            //if the computer:
            // 1. Can't play its hand and can't play its reserve it has to cut off a head
            //if all of the above are true it has to cut off a head
            else {
                player.discardHand();
                player.clearReserve(); 
                
                
                //cutheads cuts off the oldest head and creates 2 new empty heads
                game.cutHead(playerNum);

                //draw 2 cards and then play them on the new heads
                for (int i = 0; i < 2; i++) {
                    player.drawCard();
                    if (handSuit == Suit::Joker) {
                        //default game rules are that jokers placed on new heads are set to 2
                        //the value of this can be changed
                        player.getHand().setValue(newHeadJokerVal);
                    }
                    game.placeOnHead(playerNum, game.getYoungestHead() - (1-i));
                }

                cout << "(Computer) Player " << playerNum << " cut off head " << youngestHead << " and ended their turn." << endl;
                //end turn, return true since turn ended
                player.endTurn();
                return true;
            }
        } 

        //computer has valid move
        else {
            if (handSuit == Suit::Joker) {
                player.getHand().setValue(Value::Ace);
            }
            cout << "(Computer) Player " << playerNum << " plays " << player.getHand();
            cout << " on head " << bestMove << endl;
            game.placeOnHead(playerNum, bestMove);  
            hasPlayedCard = true;
        }
    }
    //end of action loop
    //we played a card if we're here so we return true
    return true;
}
