#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <climits>

#include "./card.h"
#include "./rule.h"

class Shoe {
public:
    Shoe(int);
    ~Shoe();

    void shuffle();
    bool needsShuffle() const;
    bool hasInfiniteDeck() const;
    Card* draw();
    Card* pickup(int);

private:
    const int _deckNum;
    std::vector<Card *> _cards;
};