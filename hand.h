/**
 * hand.h
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
#include <string>

#include "./card.h"

class Hand {
public:
    Hand(Card*, int = 0);
    Hand(std::vector<Card*>&, int = 0);
    virtual ~Hand();
    
    int rank() const;
    void add(Card*);
    bool isBlackjack() const;
    bool isBusted() const;
    bool isSoft() const;
    bool isSoft17() const;
    bool isPair() const;
    bool isPairOf(int) const;
    bool isDoubledown() const;
    bool canDoubleDownOrSurrender() const;
    void doubleDownWith(Card*);
    Hand* split();
    void surrender();
    bool isSurrendered() const;
    int bet() const;
    
    std::string rankString() const;
    std::string toString() const;

    int size() const { return _cards.size(); }
    
protected:

private:
    int sum() const;
    
    std::vector<Card*> _cards;
    int _bet;
    
    bool _isDoubledown;
    bool _isSurrendered;
};


