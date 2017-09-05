/**
 * dealer.h
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

#include <vector>
#include <random>
#include <string>

#include "./rule.h"
#include "./card.h"
#include "./hand.h"
#include "./lib/singleton.h"
#include "./lib/counter.h"

class Dealer : public Singleton<Dealer> {
public:
    virtual ~Dealer();

    static Dealer* create() {
        static Dealer dealer;
        return &dealer;
    }
    
    void shuffle();
    bool needsShuffle() const;
    void dealHandToSelf();
    Card* deal();
    bool hasBlackjack() const;
    void recordResult();
    void doAction();
    
    std::string toString() const;
    
protected:

private:
    Dealer();
    
    std::vector<Card*> _cards;
    
    Hand* _hand;
    
    Counter<std::string>* _resultCounter;
};
