#ifndef _PILE_H_
#define _PILE_H_
#include <vector>

class Card;

class Pile {
    std::vector<Card> pile;

    public:
    Pile();
    ~Pile();

    bool isEmpty() const;
    int getSize() const;
    void shufflePile();

    //access top card
    Card & getTopCard();
    //view top card
    const Card & peekTopCard() const;
    //moves top card to the specified pile
    void moveTopCard(Pile & pile, bool faceUp);
    //adds a copy of a card to the top of the pile
    void addCard(Card c, bool faceUp);
    //moves own contents to the given pile
    void movePile(Pile & pile, bool faceUp);
    //overwrites the contents of the pile with a standard 54 card deck
    void setDeck();
};

#endif
