/**
 * simulation.cc
 *
 * Copyright (c) 2019 Takayuki Matsumura
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include "./include/simulation.h"

Simulation::Simulation(Dealer* dealer, Player* player) {
    _dealer = dealer;
    _player = player;

    _dealer->add(player);
}

Simulation::~Simulation() {
}

void Simulation::step() {
    _player->bet(2);

    _dealer->dealHandTo(_player);
    _dealer->dealHandToSelf();

    if(_dealer->has(A)) {
        _player->doInsuranceOrNot(_dealer);
    }
    if(_dealer->hasBlackjack()) {
        if(_player->takesInsurance()) {
            _player->getPrize(1.5);
        }
    }
    else {
        _player->doAction(_dealer);
    }
    _dealer->doAction();

    _player->adjust(_dealer->hand());

    _player->recordResult(_dealer);
    _dealer->recordResult();
}

void Simulation::start() {
    int gameCounter = 0;

    std::cout << "Simulation start" << std::endl;
    while(true) {
        _dealer->shuffle();

        while(!_dealer->needsShuffle()) {
            if(Config::instance()->isDebugMode) {
                std::cout << "Game:" << gameCounter << " start" << std::endl;
            }

            step();

            if(Config::instance()->isDebugMode) {
                std::cout << std::endl;
            }

            if(++gameCounter == Config::instance()->game) goto end;
        }
    }
    end:
    std::cout << "Simulation finish" << std::endl;
    std::cout << std::endl;

    std::cout << "Simulation results" << std::endl;
    std::cout << _player->toString() << std::endl;
    std::cout << std::endl;
    std::cout << _dealer->toString() << std::endl;
}
