#pragma once

#include <vector>
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
    Card *draw();

private:
    const int _deckNum;
    std::vector<Card *> _cards;
};