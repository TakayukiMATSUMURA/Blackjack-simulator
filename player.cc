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
    _handRankAfterDoubleDownCounter = new Counter<std::string>();
    _insuranceCounter = new Counter<bool>();
    _doubledownCounter = new Counter<std::string>();
    _splitCounter = new Counter<int>();
    _surrenderCounter = new Counter<bool>();
}

Player::~Player() {
    delete _strategy;
    delete _resultCounter;
    delete _handRankCounter;
    delete _handRankAfterDoubleDownCounter;
    delete _insuranceCounter;
    delete _doubledownCounter;
    delete _splitCounter;
    delete _surrenderCounter;
}

void Player::bet(int unit) {
    _takesInsurance = false;
    
    int amount = _strategy->betAmount(unit);
    _betAmount = amount;
    _bankroll -= _betAmount;
    _totalBetAmount += _betAmount;
    
    if(Config::instance()->isDebugMode) {
        std::cout << "Player's bankroll:" << (_bankroll + _betAmount) << std::endl;
        std::cout << "Strategy:" << _strategy->toString() << std::endl;
        std::cout << "Player bets:" << _betAmount << std::endl;
    }
}

int Player::betAmount() const {
    return _betAmount;
}

void Player::receive(std::vector<Card*>& cards) {
    auto hand = new Hand(cards, _betAmount);
    _hands.push_back(hand);

    if(Config::instance()->isDebugMode) {
        std::cout << "Player's hand:" << hand->toString() << std::endl;
    }
}

void Player::getPrize(float rate, Hand* hand) {
    if(hand == nullptr) hand = _hands[0];
    
    int prize = hand->bet() * rate;
    _bankroll += prize;
    _totalPrizeAmount += prize;
    
    if(Config::instance()->isDebugMode) {
        std::cout << "Player gets " << prize  << " with " << hand->toString() << std::endl;
    }
}

void Player::doInsuranceOrNot() {
    if(!_strategy->takesInsurance()) return;
    
    _takesInsurance = true;
    _bankroll -= _betAmount / 2;
    _totalBetAmount += _betAmount / 2;
    
    _insuranceCounter->count(Dealer::instance()->hasBlackjack());
    if(Config::instance()->isDebugMode) {
        std::cout << "Player takes insurance(bet:" << (_betAmount / 2) << ")" << std::endl;
    }
}

bool Player::takesInsurance() const {
    return _takesInsurance;
}

void Player::doAction() {
    if(hasBlackjack()) return;
    
    auto dealer = Dealer::instance();
    int handCounter = 0;
    while(handCounter < _hands.size()) {
        auto currentHand = _hands[handCounter++];
        int action = -1;
        
            
        if(Config::instance()->isDebugMode) {
            std::cout << "Player's hand:" << currentHand->toString() << std::endl;
        }
        while(action != Stand && action != DoubleDown && action != Surrender && !currentHand->isBusted()) {
            action = _strategy->getAction(currentHand, dealer->upCardRank());
            if(_hands.size() > 2) {
                if(action == Surrender) action = Hit;
                if(action == DoubleDown && !Rule::instance()->DaS) action = Hit;
            }

            if(Config::instance()->isDebugMode && !currentHand->isBusted()) {
                auto actionString = action == Hit ? "Hit" :
                    action == Surrender ? "Surrender" :
                    action == Split ? "Split" :
                    action == DoubleDown ? "Double down": "Stand";
                if(!currentHand->isBusted()) {
                    std::cout << "action:" << actionString << std::endl;
                }
            }
            
            if(action == Hit) {
                currentHand->add(dealer->deal());
            }
            else if(action == DoubleDown) {
                _bankroll -= currentHand->bet();
                _totalBetAmount += currentHand->bet();
                
                currentHand->doubleDownWith(dealer->deal());
                _handRankAfterDoubleDownCounter->count(currentHand->rankString());
            }
            else if(action == Split) {
                _bankroll -= currentHand->bet();
                _totalBetAmount += currentHand->bet();
                
                if(currentHand->isPairOf(A)) {
                    action = Stand;
                    handCounter++;
                }
                
                auto newHand = currentHand->split();
                _hands.push_back(newHand);
            }
            else if(action == Surrender) {
                currentHand->surrender();
            }
            
            if(Config::instance()->isDebugMode) {
                std::cout << "hand:" << currentHand->toString() << std::endl;
            }
        }
        
        _surrenderCounter->count(action == Surrender);
    }
}

void Player::adjust() {
    auto dealer = Dealer::instance();
    if(dealer->hasBlackjack()) {
        if(hasBlackjack()) {
            getPrize(1, _hands[0]);
        }
        
        return;
    }
    
    if(hasBlackjack()) {
        getPrize(2.5, _hands[0]);
        return;
    }

    for(const auto& hand : _hands) {
        if(hand->isBusted()) continue;

        if(hand->isSurrendered()) {
            getPrize(1, hand);
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
    auto tie = "tie";
    auto win = "win";
    auto lose = "lose";
    for(const auto& hand : _hands) {
        if(dealer->hasBlackjack()) {
            if(hasBlackjack()) {
                _resultCounter->count(tie);
                _handRankCounter->count(hand->rankString());
            }
            else {
                _resultCounter->count(lose);
                _handRankCounter->count("Dealer BJ");
            }
        }
        else if(hand->isBusted()) {
            _resultCounter->count(lose);
            _handRankCounter->count(hand->rankString());
        }
        else if(hasBlackjack()) {
            _resultCounter->count(win);
            _handRankCounter->count(hand->rankString());
        }
        else if(hand->isSurrendered()) {
            _resultCounter->count(lose);
            _handRankCounter->count(hand->rankString());
        }
        else {
            if(hand->rank() > dealer->handRank() || dealer->isBusted()) {
                _resultCounter->count(win);
                if(hand->isDoubledown()) {
                    _doubledownCounter->count(win);
                }
            }
            else if(hand->rank() < dealer->handRank()) {
                _resultCounter->count(lose);
                if(hand->isDoubledown()) {
                    _doubledownCounter->count(lose);
                }
            }
            else {
                _resultCounter->count(tie);
                if(hand->isDoubledown()) {
                    _doubledownCounter->count(tie);
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
    std::string result = "############# Player ##############\n";
    result += "Result\n" + _resultCounter->toStringInDescendingOrder() + "\n\n";
    result += "Insurance accuracy\n" + _insuranceCounter->getStringPercentageOf(true) + "\n";
    result += "Surrender\n" + _surrenderCounter->getStringPercentageOf(true) + "\n\n";
    result += "Double down\n" + _doubledownCounter->toStringInDescendingOrder() + "\n\n";
    result += "Hand distribution after Double down\n" + _handRankAfterDoubleDownCounter->toStringInDescendingOrder() + "\n\n";
    
    result += "Split\n" + _splitCounter->toStringInDescendingOrder() + "\n\n";
    result += "Hand distribution\n" + _handRankCounter->toStringInDescendingOrder() + "\n\n";
    auto ev = expectedValue() > 0 ? "+" + std::to_string(expectedValue() * 100) :
        std::to_string(expectedValue() * 100);
    result += "Expected value\n" + ev + "%\n";
    return result;
}
