/**
 * hand.cc
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
#include "./hand.h"

Hand::Hand(std::vector<Card*>& cards) {
    _cards = cards;
}

Hand::~Hand() {
}

int Hand::rank() const {
    if(isBlackjack()) return 21;

    int result = sum();
    if(isSoft()) result += 10;
    
    return result;
}

bool Hand::isBlackjack() const {
    return _cards.size() == 2 &&
        ((_cards[0]->rank() == A && _cards[1]->rank() == T) ||
         (_cards[0]->rank() == T && _cards[1]->rank() == A));
}

bool Hand::isBusted() const {
    return rank() > 21;
}

bool Hand::isSoft() const {
    bool hasAce = false;
    for(const auto& card : _cards) {
        if(card->rank() == A) {
            hasAce = true;
            break;
        }
    }
    return hasAce && sum() < 11;
}

bool Hand::isSoft17() const {
    return isSoft() && rank() == 17;
}

void Hand::add(Card* card) {
    _cards.push_back(card);
}

int Hand::sum() const {
    int result = 0;
    for(const auto& card : _cards) {
        result += card->rank();
    }
    return result;
}
