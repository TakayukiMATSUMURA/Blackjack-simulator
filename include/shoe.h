#pragma once

#include <vector>
#include <random>

#include "./card.h"
#include "./rule.h"

class Shoe {
public:
    Shoe(int);
    ~Shoe();

    void shuffle();
    bool needsShuffle() const;
    Card* draw();

    std::vector<Card*> _cards;
};