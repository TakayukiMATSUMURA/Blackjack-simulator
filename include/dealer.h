#pragma once

#include <vector>
#include <random>
#include <string>
#include <iostream>

#include "./rule.h"
#include "./card.h"
#include "./hand.h"
#include "./player.h"
#include "./shoe.h"
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

    void dealHandToSelf(Card*, Card*);
    bool has(int) const;
    bool hasBlackjack() const;
    bool isBusted() const;
    void recordResult();
    void resetHand();
    void doAction(Player*, Shoe*);

    int upCardRank() const;
    Hand* hand() const { return _hand; }

    void pay(Player*, float);

    std::string toString() const;

private:
    Card* _upCard;
    Card* _holeCard;
    Hand* _hand;
    Counter<std::string>* _handRankCounter;
    Counter<std::string>* _handRankCounterForEachRanks[10];
    Counter<int>* _allHandCardCounter;
    Counter<int>* _patHandCardCounter;
    Counter<int>* _bustedHandCardCounter;
};
