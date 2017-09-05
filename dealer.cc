/**
 * dealer.cc
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
#include "./dealer.h"

Dealer::Dealer() {
    _cards = Card::getDeck(Rule::instance()->deck);
    _hand = nullptr;
    _resultCounter = new Counter<std::string>();
}

Dealer::~Dealer() {
    if(_hand != nullptr) {
        delete _hand;
    }
    
    delete _resultCounter;
}

void Dealer::shuffle() {
    _cards = Card::getDeck(Rule::instance()->deck);
    
    std::random_device rnd;
    std::mt19937_64 mt(rnd());
    std::shuffle(std::begin(_cards), std::end(_cards), mt);
}

bool Dealer::needsShuffle() const {
    return _cards.size() <=
        Rule::instance()->deck * 52 * (100 - Rule::instance()->penetration) / 100;
}

void Dealer::dealHandToSelf() {
    if(_hand != nullptr) {
        delete _hand;
    }
    
    std::vector<Card*> cards;
    for(int i = 0; i < 2; i++) {
        cards.push_back(deal());
    }

    _hand = new Hand(cards);
}

Card* Dealer::deal() {
    auto card = _cards.back();
    _cards.pop_back();
    return card;
}

bool Dealer::hasBlackjack() const {
    return _hand->isBlackjack();
}

void Dealer::recordResult() {
    std::string result;
    if(_hand->isBlackjack()) result = "BJ";
    else if(_hand->isBusted()) result = "Bust";
    else result = std::to_string(_hand->rank());

    _resultCounter->count(result);
}

void Dealer::doAction() {
    while(_hand->rank() < 17 || (_hand->isSoft17() && Rule::instance()->hitSoft17)) {
        _hand->add(deal());
    }
}


std::string Dealer::toString() const {
    return _resultCounter->toStringInDescendingOrder();
}
