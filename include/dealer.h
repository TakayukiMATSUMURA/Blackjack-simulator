#pragma once

#include <vector>
#include <random>
#include <string>
#include <iostream>

#include "./rule.h"
#include "./card.h"
#include "./hand.h"
#include "./player.h"
#include "./lib/singleton.h"
#include "./lib/counter.h"

class Dealer {
public:
    Dealer();
    virtual ~Dealer();

    static Dealer* create() {
        static Dealer dealer;
        return &dealer;
    }

    void shuffle();
    bool needsShuffle() const;
    void dealHandTo(Player*);
    void dealHandToSelf();
    Card* deal();
    bool has(int) const;
    bool hasBlackjack() const;
    bool isBusted() const;
    void recordResult();
    void doAction(Player*);

    int upCardRank() const;
    Hand* hand() const { return _hand; }

    void pay(Player*, float);

    std::string toString() const;

private:
    std::vector<Card*> _cards;

    Card* _upCard;
    Card* _holeCard;
    Hand* _hand;
    Counter<std::string>* _handRankCounter;
    Counter<std::string>* _handRankCounterForEachRanks[10];
};
