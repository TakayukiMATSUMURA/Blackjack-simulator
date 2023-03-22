#pragma once

#include <vector>
#include <string>

enum {
    A = 1,
    T = 10,
};

class Card {
public:
    ~Card() {}

    static Card* getInstance(std::string);
    static std::vector<Card*> getDeck(int = 1);

    std::string toString() const { return _str; }
    const int rank;
    const int suit;

private:
    Card(std::string);
    const std::string _str;
};
