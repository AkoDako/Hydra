#include "SuitValue.h"
#include <iostream>
using namespace std;

ostream & operator<<(ostream & out, Suit & suit) {
    out << static_cast<char>(suit);
    return out;
}

ostream & operator<<(ostream & out, Value & value) {
    switch (value) {
        case Value::Ace:
            out << "A";
            break;
        case Value::Jack:
            out << "J";
            break;
        case Value::Queen:
            out << "Q";
            break;
        case Value::King:
            out << "K";
            break;
        default:
            out << static_cast<int>(value);
    }
    return out;
}
