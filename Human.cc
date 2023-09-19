#include "Human.h"
#include "Game.h"
#include "Player.h"
#include "Card.h"
#include "Pile.h"
#include "SuitValue.h"
#include "Computer.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

//value of jokers used to make new heads
//the value is const and there isn't a particular need to keep it private, since it's a global game rule
//derived subclasses also cannot access this value if it is a private member
const Value newHeadJokerVal = Value::Two;

bool Human::takeAction(Player & player, Game & game) {
    int liveHeads = game.getLiveHeads();
    int playerNum = player.getNumber();
    int oldestHead = game.getOldestHead();
    int youngestHead = game.getYoungestHead();
    bool testingEnabled = game.isTestingEnabled(); 
    bool hasPlayedCard = false;
    string move;

    //if a player's only remaining card is in reserve they put it in their discard pile and end their turn
    //this ends the player's turn so we return true
    if (player.drawPileSize() == 0 && player.reserveSize() == 1 && player.discardPileSize() == 0 && player.handSize() == 0) {
        player.clearReserve();
        player.endTurn();
        return true;
    }

    //loop until the player plays a card or ends their turn
    while (!hasPlayedCard) {
        //player's hand would only be empty if they swapped with reserve and haven't drawn a card yet
        //if their draw pile and discard pile are empty, and their only card is in discard, they move their
        //reserve to discard and end their turn (so we return true)
        if (player.handSize() == 0) {
            if (player.drawPileSize() == 0 && player.discardPileSize() == 0 && player.reserveSize() == 1) {
                player.clearReserve();
                player.endTurn();
                return true;
            }
            else {
                player.drawCard();
            }
        }
            
        Card & hand = player.getHand();

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
            }
        }
            
        //if theres no heads the player creates the first head by playing their hand and immediately ends their turn
        if (liveHeads == 0) {
            //read in a value if the player is holding a joker
            if (hand.getSuit() == Suit::Joker) {
                string input;
                cout << "Joker value?" << endl;
                try {
                    Value jokerVal = readInValue(false);
                    hand.setValue(jokerVal);
                }
                //reset turn if we didn't get a valid value
                catch (invalid_argument & e) {
                    cout << e.what() << endl << endl;
                    return false;
                }
            }
            game.init();
            //player's turn ends, we return true
            return true;
        }
            
        cout << endl;
        cout << "Player " << playerNum << ", you are holding a ";
        if (hand.getSuit() == Suit::Joker) {
            cout << "Joker";
        }
        else {
            cout << hand;
        } 
        cout << ".  Your move?" << endl;
        
        //get input 
        if (!(cin >> move)) {
            throw runtime_error("Failed to read in input");
        }
        
        if (move == "leave") {
            if (game.areComputersEnabled()) {
                cout << "Giving control of Player " << playerNum << " to computer." << endl;
                player.setStrategy(make_shared<Computer>());

            }
            else {
                cout << "Computer players are not enabled." << endl;
            }
            //take action again (called will call strat->takeAction again which will dynamically change behaviour)
            return false;
        }

        int headMove;
        try {
            //check if input was an integer
            headMove = stoi(move);    
        }
        //reset action on invalid input (e.g. return false and the caller will call takeAction again)
        catch (invalid_argument & i) {
            cout << "Please input a valid integer" << endl;
            return false;
        }
            
        //validMove represents if the player can play their hand on the specified head
        //hasValidMoves represents if the player can play their hand on any head
        bool validMove = false;
        bool hasValidMoves = false;

        // Perform the command
        //0 is the reserve command
        //reserving makes a player take another action
        if (headMove == 0) {
            player.placeInReserve();
            //returning false since we didnt play a card or end our turn early yet
            return false;
        }

        //check if specified head exists and is alive
        //take action again 
        else if (headMove > youngestHead || headMove < oldestHead) {
            cout << "Invalid move: Head is not alive/does not exist" << endl;
            return false;
        }

        //the number given is the number of a valid living head
        //we read in joker value (if necessary) and then validate the move
        else  {
            if (hand.getSuit() == Suit::Joker) {//if player is trying to play joker get a value
                string input;
                cout << "Joker value?" << endl;
                try {
                    Value jokerVal = readInValue(false);
                    hand.setValue(jokerVal);
                }
                catch (invalid_argument &e) {
                    cout << "Invalid joker value." << endl << endl;
                    return false;
                }
            }
                
            //update val/suit since we may have changed player's hand
            Value val = hand.getValue();
            Suit suit = hand.getSuit();
            Value headVal = game.getHeadValue(headMove);
            //check if playing hand on the head with the given number is valid
            if (val <= headVal || headVal == Value::Ace) {
                validMove = true;
                hasValidMoves = true;
            }
                    
            //check if player has any possible moves with their hand
            else if (suit == Suit::Joker) {
                hasValidMoves = true;
            }

            else {
                //check if any head has a value higher than or equal to the value of 
                //the card the player is currently holding, or if the head's top card is an ace
                for (int i = 1; i < youngestHead+1; i++) {
                    if (game.isHeadAlive(i)) {
                        Value currentHeadVal = game.getHeadValue(i);
                        if (val <= currentHeadVal || currentHeadVal == Value::Ace) {
                            hasValidMoves = true;
                            break;
                        }
                    }
                } 
            }
            
            //play the player's hand on the specified head if it's a valid move
            if (validMove) {
                //playing a card of equal value on a head ends a players turn immediately
                //set endTurn flag in player and return true since we end turn
                if (game.getHeadValue(headMove) == hand.getValue()) {
                    game.placeOnHead(playerNum, headMove);
                    player.endTurn();
                    return true;
                }
                else {
                    game.placeOnHead(playerNum, headMove);
                    hasPlayedCard = true;
                }
            }

            //can't play the player's hand on the specified head
            else {
                //check if the player wanted to cut off a head
                if (headMove == oldestHead) {
                    if (hasValidMoves) {
                        if (player.getHand().getSuit() == Suit::Joker) {
                            cout << "Cannot cut off head: player has a Joker in hand" << endl;
                        }
                        else {
                            cout << "Cannot cut off head: player has other valid moves" << endl;
                        }
                        //take another action
                        return false;
                    }
                    //cut off the head and end the player's turn 
                    //raises player's endofTurn bool to true and then returns true to end turn
                    else {
                        player.discardHand();
                        player.clearReserve(); 
                
                        //cutheads cuts off the oldest and creates 2 new empty heads
                        game.cutHead(playerNum);
                        //draw 2 cards and then play them on the new heads
                        for (int i = 0; i < 2; i++) {
                            player.drawCard();
                            if (hand.getSuit() == Suit::Joker) {
                                //default game rules are that jokers placed on new heads are set to 2
                                //the value of this can easily be changed in playerStrategy
                                hand.setValue(newHeadJokerVal);
                            }
                            game.placeOnHead(playerNum, game.getYoungestHead() - (1-i));
                        }
                        //end player's turn
                        player.endTurn();
                        return true;
                    }
                }
            }   
        }
    }
    //end of the action loop (e.g. we played a card)
    return true;
}
