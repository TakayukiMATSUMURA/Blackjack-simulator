/**
 * player.cc
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
#include "./player.h"
#include "./dealer.h"

Player::Player(int bankroll, IStrategy* strategy) {
    _bankroll = bankroll;
    _strategy = strategy;

    _totalBetAmount = 0;
    _totalPrizeAmount = 0;
    
    _resultCounter = new Counter<std::string>();
    _handRankCounter = new Counter<std::string>();
    _insuranceCounter = new Counter<bool>();
    _doubledownCounter = new Counter<std::string>();
    _splitCounter = new Counter<int>();
    _surrenderCounter = new Counter<bool>();
}

Player::~Player() {
    delete _strategy;
    delete _resultCounter;
    delete _doubledownCounter;
    delete _splitCounter;
}

void Player::bet(int unit) {
    int amount = _strategy->betAmount(unit);
    _betAmount = amount;
    _bankroll -= _betAmount;
    _totalBetAmount += _betAmount;
    
    _takesInsurance = false;
}

int Player::betAmount() const {
    return _betAmount;
}

void Player::receive(std::vector<Card*>& cards) {
    auto hand = new Hand(cards, _betAmount);
    _hands.push_back(hand);
}

void Player::getPrize(float rate, Hand* hand) {
    int prize = hand == nullptr ? _betAmount : hand->bet() * rate;
    _bankroll += prize;
    _totalPrizeAmount += prize;
}

void Player::doInsuranceOrNot() {
    if(!_strategy->takesInsurance()) return;
    
    _bankroll -= _betAmount / 2;
    _totalBetAmount += _betAmount / 2;
    
    _insuranceCounter->count(Dealer::instance()->hasBlackjack());
}

bool Player::takesInsurance() const {
    return _takesInsurance;
}

void Player::doAction() {
    auto dealer = Dealer::instance();
    int handCounter = 0;
    while(handCounter < _hands.size()) {
        auto currentHand = _hands[handCounter++];
        int action = -1;
        while(action != Stand && action != DoubleDown && action != Surrender) {
            action = _strategy->getAction(currentHand, dealer->upCardRank());
            if(_hands.size() > 2) {
                if(action == Surrender) action = Hit;
                if(action == DoubleDown) action = Hit;
            }
            
            if(action == Hit) {
                currentHand->add(dealer->deal());
            }
            else if(action == DoubleDown) {
                _bankroll -= currentHand->bet();
                _totalBetAmount += currentHand->bet();
                
                currentHand->doubleDownWith(dealer->deal());
            }
            else if(action == Split) {
                _bankroll -= currentHand->bet();
                _totalBetAmount += currentHand->bet();
                
                auto newHand = currentHand->split();
                _hands.push_back(newHand);
            }
            else if(action == Surrender) {
                currentHand->surrender();
            }
        }
        
        _surrenderCounter->count(action == Surrender);
    }
}

void Player::adjust() {
    auto dealer = Dealer::instance();
    
    for(const auto& hand : _hands) {
        if(hand->isBusted()) continue;
        if(hand->isSurrendered()) {
            getPrize(0.5, hand);
        }
        else if(hand->rank() > dealer->handRank() || dealer->isBusted()) {
            getPrize(2, hand);
        }
        else if(hand->rank() == dealer->handRank()) {
            getPrize(1, hand);
        }
        
    }
}

bool Player::hasBlackjack() const {
    return _hands.size() == 1 && _hands[0]->isBlackjack();
}

void Player::count(Card* card) {
    _strategy->count(card);
}

void Player::onShuffle() {
    _strategy->reset();
}

void Player::recordResult() {
    if(_hands.size() > 1) {
        _splitCounter->count(_hands.size() - 1);
    }
    
    auto dealer = Dealer::instance();
    
    for(const auto& hand : _hands) {
        if(dealer->hasBlackjack()) {
            if(hasBlackjack()) {
                _resultCounter->count("tie");
                _handRankCounter->count(hand->rankString());
            }
            else {
                _resultCounter->count("lose");
                _handRankCounter->count("Dealer BJ");
            }
        }
        else if(hand->isBusted()) {
            _resultCounter->count("lose");
            _handRankCounter->count(hand->rankString());
        }
        else if(hasBlackjack()) {
            _resultCounter->count("win");
            _handRankCounter->count(hand->rankString());
        }
        else if(hand->isSurrendered()) {
            _resultCounter->count("lose");
            _handRankCounter->count(hand->rankString());
        }
        else {
            if(hand->rank() > dealer->handRank() || dealer->isBusted()) {
                _resultCounter->count("win");
                if(hand->isDoubledown()) {
                    _doubledownCounter->count("win");
                }
            }
            else if(hand->rank() < dealer->handRank()) {
                _resultCounter->count("lose");
                if(hand->isDoubledown()) {
                    _doubledownCounter->count("lose");
                }
            }
            else {
                _resultCounter->count("tie");
                if(hand->isDoubledown()) {
                    _doubledownCounter->count("tie");
                }
            }
            
            if(hand->rank() == 21) {
                _handRankCounter->count("21");
            }
            else {
                _handRankCounter->count(hand->rankString());
            }
        }
        
        delete hand;
    }
    _hands.clear();
}

float Player::expectedValue() const {
    return (float)_totalPrizeAmount / _totalBetAmount - 1;
}

std::string Player::toString() const {
    std::string result = "############# Player ###############\n";
    result += "Result\n" + _resultCounter->toStringInDescendingOrder() + "\n\n";
    result += "Insurance accuracy\n" + _insuranceCounter->getStringPercentageOf(true) + "\n";
    result += "Surrender\n" + _surrenderCounter->getStringPercentageOf(true) + "\n\n";
    result += "Double down\n" + _doubledownCounter->toStringInDescendingOrder() + "\n\n";
    result += "Split\n" + _splitCounter->toStringInDescendingOrder() + "\n\n";
    result += "Hand distribution\n" + _handRankCounter->toStringInDescendingOrder() + "\n\n";
    auto ev = expectedValue() > 0 ? "+" + std::to_string(expectedValue() * 100) :
        std::to_string(expectedValue() * 100);
    result += "Expected value\n" + ev + "%\n";
    return result;
}
