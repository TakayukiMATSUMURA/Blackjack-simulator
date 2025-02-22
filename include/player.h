#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include "./hand.h"
#include "./config.h"
#include "./shoe.h"
#include "./strategies/istrategy.h"
#include "./lib/counter.h"

class Dealer;

class Player
{
public:
    Player(int, IStrategy *);
    virtual ~Player();

    void bet(int);
    int betAmount() const;
    void receive(std::vector<Card *> &);
    void getPrize(float, Hand * = nullptr);
    void doInsuranceOrNot(Dealer *);
    bool takesInsurance() const;
    void doAction(Dealer *, Shoe *);
    void adjust(Hand *);
    bool hasBlackjack() const;

    void count(Card *);
    void onShuffle();

    void recordResult(Dealer *);
    void resetHands();

    float houseEdge(int) const;
    std::string toString() const;

private:
    int _bankroll;
    int _betAmount;
    bool _takesInsurance;
    IStrategy *_strategy;

    std::vector<Hand *> _hands;

    int _totalBetAmount;
    int _totalPrizeAmount;

    Counter<std::string> *_resultCounter;
    Counter<std::string> *_handRankCounter;
    Counter<std::string> *_handRankAfterDoubleDownCounter;
    Counter<bool> *_insuranceCounter;
    Counter<std::string> *_doubledownCounter;
    Counter<int> *_splitCounter;
    Counter<int> *_cardCountOfHandCounter;
    Counter<double> *_diffBetweenShufflesCounter;
    std::vector<int> _buffer;
};
