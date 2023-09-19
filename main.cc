#include "Card.h"
#include "Pile.h"
#include "Game.h"
#include "Player.h"
#include "Human.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>

using namespace std;

int main(int argc, char *argv[]) {
    bool testingEnabled = false;
    bool enhancementsEnabled = false;
    bool customDrawSize = false;

    int playerCount = 0;
    //default number of cards players start with in their draw pile is 54
    int startingCards = 54;
    for (int i = 1; i < argc; i++) {
        string temp = argv[i];
        if (temp == "-testing") {
            testingEnabled = true;
        }
        else if (temp == "-computers") {
            enhancementsEnabled = true;
        }
        else if (temp == "-customsize") {
            customDrawSize = true;
        }
        else {
            cout << "Invalid command line argument " << argv[i] << " ignored." << endl;
        }
    }
   
    cout << "How many players?" << endl;
    cin >> playerCount;
    if (cin.fail() || cin.eof()) {
        cout << "No input read in, quitting" << endl;
        return 1;
    }
    else if (playerCount < 2) {
        cout << "Less than 2 players specified, defaulting to 2" << endl;
        playerCount = 2;
    }
    
    if (customDrawSize) {
        cout << "How many cards do players start with? (default is 54)" << endl;
        cin >> startingCards;
    }

    if (startingCards <= 0 || startingCards > 54) {
        cout << "Can't have players start with 0 or less cards or more than 54 cards; defaulted to 54" << endl;
        startingCards = 54;
    }

    Game g = Game(playerCount, testingEnabled, enhancementsEnabled, startingCards);

    if (enhancementsEnabled) {
        string command;
        while (true) {
            cout << "Enter number of player to be controlled by computer, or \"play\" to start game" << endl;
            if (!(cin >> command)) {
                cout << "Failed to read in input, exiting program" << endl;
                break;
            }
            if (command == "play") {
                break;
            }
            try {
                int num = stoi(command);
                if (num >= 1 && num <= playerCount) {
                    g.toggleComputer(num);
                }
                else {
                    cout << "Please enter the number of a valid player" << endl;
                }
            }
            catch (invalid_argument & e) {
                cout << "Please enter the number of a player or \"play\"" << endl;
            }
        }
    }

    while (true) {
        try {
            if (g.processRound()) {//if someone won
                break;
            }
        }
        catch (runtime_error & e) {
            cout << e.what() << endl;
            break;
        }
    }
}
