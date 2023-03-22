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
#include "./dealer.h"

Hand::Hand(Card* card, int bet) {
    _cards.push_back(card);
    _bet = bet;
    _isSurrendered = false;
}

Hand::Hand(std::vector<Card*>& cards, int bet) {
    _cards = cards;
    _isDoubledown = false;
    _bet = bet;
    _isSurrendered = false;
}

Hand::~Hand() {
}

int Hand::rank() const {
    if(isBlackjack()) return 21;
    return sum() + (isSoft() ? 10 : 0);
}

void Hand::add(Card* card) {
    _cards.push_back(card);
}

bool Hand::isBlackjack() const {
    return _cards.size() == 2 && ((_cards[0]->rank == A && _cards[1]->rank == T) || (_cards[0]->rank == T && _cards[1]->rank == A));
}

bool Hand::isBusted() const {
    return rank() > 21;
}

bool Hand::isSoft() const {
    auto hasAce = std::any_of(std::begin(_cards), std::end(_cards), [&](const Card* card) { return card->rank == A;});
    return hasAce && sum() <= 11;
}

bool Hand::isSoft(int r) const {
    return isSoft() && rank() == r;
}

bool Hand::isPair() const {
    return _cards.size() == 2 && _cards[0]->rank == _cards[1]->rank;
}

bool Hand::isPairOf(int rank) const {
    return isPair() && _cards[0]->rank == rank;
}

bool Hand::isDoubledown() const {
    return _isDoubledown;
}

bool Hand::canDoubleDownOrSurrender() const {
    return _cards.size() == 2;
}

void Hand::doubleDownWith(Card* card) {
    add(card);
    _isDoubledown = true;
    _bet *= 2;
}

Hand* Hand::split(Card* card0, Card* card1) {
    auto card = _cards.back();
    _cards.pop_back();
    auto newHand = new Hand(card, bet());
    add(card0);
    newHand->add(card1);
    return newHand;
}

void Hand::surrender() {
    _isSurrendered = true;
}

bool Hand::isSurrendered() const {
    return _isSurrendered;
}

int Hand::bet() const {
    return _bet;
}

bool Hand::loses(Hand* dealersHand) const {
    if(isBusted() || isSurrendered() || (dealersHand->isBlackjack() && !isBlackjack())) {
        return true;
    }
    return !dealersHand->isBusted() && rank() < dealersHand->rank();
}

bool Hand::winsAgainst(Hand* dealersHand) const {
    if(loses(dealersHand)) {
        return false;
    }
    return dealersHand->isBusted() || (!isBusted() && rank() > dealersHand->rank());
}

std::string Hand::rankString() const {
    if(isBlackjack()) return "BJ";
    else if(isBusted()) return "Bust";
    return std::to_string(rank());
}

std::string Hand::toString() const {
    std::string result;
    for(const auto& card : _cards) {
        result += card->toString();
    }

    result += ":" + rankString();
    if(bet() > 0) {
        result += " " + std::to_string(bet()) + "bet";
    }
    return result;
}

int Hand::sum() const {
    return std::accumulate(std::begin(_cards), std::end(_cards), 0, [](int sum, Card* card) { return sum + card->rank; });
}
