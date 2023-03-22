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
    _cardCountOfHandCounter = new Counter<int>();
    _diffBetweenShufflesCounter = new Counter<int>();
}

Player::~Player() {
    delete _strategy;
    delete _resultCounter;
    delete _handRankCounter;
    delete _handRankAfterDoubleDownCounter;
    delete _insuranceCounter;
    delete _doubledownCounter;
    delete _splitCounter;
    delete _cardCountOfHandCounter;
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

void Player::doInsuranceOrNot(Dealer* dealer) {
    if(!_strategy->takesInsurance()) return;

    _takesInsurance = true;
    _bankroll -= _betAmount / 2;
    _totalBetAmount += _betAmount / 2;

    _insuranceCounter->count(dealer->hasBlackjack());
    if(Config::instance()->isDebugMode) {
        std::cout << "Player takes insurance(bet:" << (_betAmount / 2) << ")" << std::endl;
    }
}

bool Player::takesInsurance() const {
    return _takesInsurance;
}

void Player::doAction(Dealer* dealer) {
    if(hasBlackjack()) return;
    int handCounter = 0;
    while(handCounter < _hands.size()) {
        auto currentHand = _hands[handCounter++];
        Action action = Action::Invalid;

        if(Config::instance()->isDebugMode) {
            std::cout << "Player's hand:" << currentHand->toString() << std::endl;
        }
        while(action != Action::Stand && action != Action::DoubleDown && action != Action::Surrender && !currentHand->isBusted()) {
            action = _strategy->getAction(currentHand, dealer->upCardRank());
            if(_hands.size() > 2) {
                if(action == Action::Surrender) action = Action::Hit;
                if(action == Action::DoubleDown && !Rule::instance()->DaS) action = Action::Hit;
            }

            if(Config::instance()->isDebugMode && !currentHand->isBusted()) {
                auto actionString = action == Action::Hit ? "Hit" :
                    action == Action::Surrender ? "Surrender" :
                    action == Action::Split ? "Split" :
                    action == Action::DoubleDown ? "Double down": "Stand";
                std::cout << "action:" << actionString << std::endl;
            }

            switch(action) {
                case Action::Hit:
                    currentHand->add(dealer->deal());
                    break;
                case Action::DoubleDown:
                    _bankroll -= currentHand->bet();
                    _totalBetAmount += currentHand->bet();

                    currentHand->doubleDownWith(dealer->deal());
                    _handRankAfterDoubleDownCounter->count(currentHand->rankString());
                    break;
                case Action::Split: {
                    _bankroll -= currentHand->bet();
                    _totalBetAmount += currentHand->bet();

                    if(currentHand->isPairOf(A)) {
                        action = Action::Stand;
                        handCounter++;
                    }

                    auto newHand = currentHand->split(dealer->deal(), dealer->deal());
                    _hands.push_back(newHand);
                    break;
                }
                case Action::Surrender:
                    currentHand->surrender();
                    break;
                default:
                    break;
            }

            if(Config::instance()->isDebugMode) {
                std::cout << "hand:" << currentHand->toString() << std::endl;
            }
        }
    }
}

void Player::adjust(Hand* dealersHand) {
    if(dealersHand->isBlackjack()) {
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

        if(hand->winsAgainst(dealersHand)) {
            getPrize(2, hand);
        }
        else if(hand->loses(dealersHand)) {
            if(hand->isSurrendered()) {
                getPrize(0.5, hand);
            }
        }
        else {
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
    _buffer.push_back(_bankroll);

    if(_buffer.size() == Config::instance()->N){
        float sum = std::accumulate(_buffer.begin(), _buffer.end(), 0);
        auto average = std::round(sum / Config::instance()->N);
        _diffBetweenShufflesCounter->count(average);
        _buffer.clear();
    }

    _bankroll = 0;
}

void Player::recordResult(Dealer* dealer) {
    if(_hands.size() > 1) {
        _splitCounter->count(_hands.size() - 1);
    }

    for(const auto& hand : _hands) {
        _cardCountOfHandCounter->count(hand->size());

        if(!dealer->hasBlackjack()) {
            _handRankCounter->count(hand->rankString());
        }
        std::string result;
        if(hand->loses(dealer->hand())) {
            result = "lose";
        }
        else if(hand->winsAgainst(dealer->hand())) {
            result = "win";
        }
        else {
            result = "tie";
        }
        _resultCounter->count(result);
        if(hand->isDoubledown()) {
            _doubledownCounter->count(result);
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
    result += "Insurance accuracy\n" + _insuranceCounter->getStringPercentageOf(true) + "\n\n";
    result += "Double down\n" + _doubledownCounter->toStringInDescendingOrder() + "\n\n";
    result += "Hand distribution after Double down\n" + _handRankAfterDoubleDownCounter->toStringInDescendingOrder() + "\n\n";

    result += "Split\n" + _splitCounter->toStringInDescendingOrder() + "\n\n";
    result += "Cards per hand\n" + _cardCountOfHandCounter->toStringInDescendingOrder() + "\n\n";
    result += "Hand distribution\n" + _handRankCounter->toStringInDescendingOrder() + "\n\n";
    result += "Bankroll distribution(per shoe)\n" + _diffBetweenShufflesCounter->toStringInDescendingOrder() + "\n\n";
    _diffBetweenShufflesCounter->saveDataTo("data/bankroll_distribution.csv");
    auto ev = expectedValue() > 0 ? "+" + std::to_string(expectedValue() * 100) :
        std::to_string(expectedValue() * 100);
    result += "Expected value\n" + ev + "%\n";
    return result;
}
