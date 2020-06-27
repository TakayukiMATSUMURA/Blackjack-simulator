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
    _handRankCounter = new Counter<std::string>();
    _counter = nullptr;
    
    for(int i = 0; i < 10; i++) {
        _handRankCounterForEachRanks[i] = new Counter<std::string>();
    }
}

Dealer::~Dealer() {
    if(_hand != nullptr) {
        delete _hand;
    }
    
    delete _handRankCounter;

    for(const auto& counter : _handRankCounterForEachRanks) {
        delete counter;
    }
}

void Dealer::shuffle() {
    _cards = Card::getDeck(Rule::instance()->deck);
    
    std::random_device rnd;
    std::mt19937_64 mt(rnd());
    std::shuffle(std::begin(_cards), std::end(_cards), mt);

    if(_counter != nullptr) {
        _counter->onShuffle();
    }
}

bool Dealer::needsShuffle() const {
    return _cards.size() <=
        Rule::instance()->deck * 52 * (100 - Rule::instance()->penetration) / 100;
}

void Dealer::dealHandTo(Player* player) {
    std::vector<Card*> cards;
    for(int i = 0; i < 2; i++) {
        cards.push_back(deal());
    }
    
    player->receive(cards);
}

void Dealer::dealHandToSelf() {
    if(_hand != nullptr) {
        delete _hand;
    }
    
    std::vector<Card*> cards;
    _upCard = deal();
    cards.push_back(_upCard);
    _holeCard = _cards.back();
    _cards.pop_back();
    cards.push_back(_holeCard);
    
    _hand = new Hand(cards);

    if(Config::instance()->isDebugMode) {
        std::cout << "Dealer's upcard:" << _upCard->toString() << std::endl;
    }
}

Card* Dealer::deal() {
    auto card = _cards.back();
    _cards.pop_back();
    
    if(_counter != nullptr) {
        _counter->count(card);
    }
    
    return card;
}

bool Dealer::has(int rank) const {
    return _upCard->rank() == rank;
}

bool Dealer::hasBlackjack() const {
    return _hand->isBlackjack();
}

bool Dealer::isBusted() const {
    return _hand->isBusted();
}

void Dealer::recordResult() {
    _handRankCounter->count(_hand->rankString());
    auto index = _upCard->rank() == A ? 9 : _upCard->rank() - 2;
    _handRankCounterForEachRanks[index]->count(_hand->rankString());
}

void Dealer::doAction() {
    if(_counter != nullptr) {
        _counter->count(_holeCard);
    }
    
    while(_hand->rank() < 17 || (_hand->isSoft17() && Rule::instance()->hitSoft17)) {
        _hand->add(deal());
    }

    if(Config::instance()->isDebugMode) {
        std::cout << "Dealer's hand:" << _hand->toString() << std::endl;
    }
}

int Dealer::upCardRank() const {
    return _upCard->rank();
}

void Dealer::add(Player* counter) {
    _counter = counter;
}

std::string Dealer::toString() const {
    std::string result = "############# Dealer ##############\n";
    if(!Config::instance()->displaysDealerDetails) {
        result += "Hand distribution\n" + _handRankCounter->toStringInDescendingOrder() + "\n";
    }
    else {
        for(int i = 0; i < 10; i++) {
            result += "Hand distribution(upcard:" + (i == 9 ? "A" : std::to_string(i + 2)) + ")\n";
            result += _handRankCounterForEachRanks[i]->toStringInDescendingOrder() + "\n\n";
        }
        
        result += "Hand distribution(all)\n" + _handRankCounter->toStringInDescendingOrder();
    }
    return result;
}
