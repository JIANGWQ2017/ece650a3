
#include "EIterator.h"

EIterator::EIterator(std::string input) {
    if (input[0] == '{') {
        this->cursor = 1;
        this->input = input;
    } else {
        throw "Error: *Unexpected input.";
    }

}

std::pair<int, int> EIterator::getNext() {
    int first = 0, second = 0;
    int *which = &first;
    while(this->input[this->cursor]!='>'){
        switch (this->input[cursor]) {
            case '<':
                which = &first;
                break;
            case ',':
                which = &second;
                break;
            default:  // number
                *which = (*which * 10) + (input[cursor] - '0');
        }
        cursor++;

    }
    cursor++;
    if (this->input[cursor] == ',') {
        cursor++;
    }


    return std::pair<int, int>{first, second};
}

bool EIterator::hasNext() {
    return this->input[this->cursor] != '}';
}
