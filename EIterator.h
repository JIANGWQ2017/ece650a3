
#ifndef A2ECE650_EITERATOR_H
#define A2ECE650_EITERATOR_H

#include <string>

class EIterator {
private:
    std::string input;
    int cursor = 0;

public:
    EIterator(std::string);
    std::pair<int, int> getNext();
    bool hasNext();


};


#endif //A2ECE650_EITERATOR_H
