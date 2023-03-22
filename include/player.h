/**
 * player.h
 *
 * Copyright (C) 2017 Takayuki MATSUMURA
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * See LICENSE file on the base directory for more information.
 *
 */
/**********************************************************************/
#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include "./hand.h"
#include "./config.h"
#include "./strategies/istrategy.h"
#include "./lib/counter.h"
class Dealer;

class Player {
public:
    Player(int, IStrategy*);
    virtual ~Player();

    void bet(int);
    int betAmount() const;
    void receive(std::vector<Card*>&);
    void getPrize(float, Hand* = nullptr);
    void doInsuranceOrNot(Dealer*);
    bool takesInsurance() const;
    void doAction(Dealer*);
    void adjust(Hand*);
    bool hasBlackjack() const;

    void count(Card*);
    void onShuffle();

    void recordResult(Dealer*);

    float expectedValue() const;
    std::string toString() const;

private:
    int _bankroll;
    int _betAmount;
    bool _takesInsurance;
    IStrategy* _strategy;

    std::vector<Hand*> _hands;

    int _totalBetAmount;
    int _totalPrizeAmount;

    Counter<std::string>* _resultCounter;
    Counter<std::string> * _handRankCounter;
    Counter<std::string> * _handRankAfterDoubleDownCounter;
    Counter<bool>* _insuranceCounter;
    Counter<std::string>* _doubledownCounter;
    Counter<int>* _splitCounter;
    Counter<int>* _cardCountOfHandCounter;
    Counter<int>* _diffBetweenShufflesCounter;
    std::vector<int> _buffer;
};
