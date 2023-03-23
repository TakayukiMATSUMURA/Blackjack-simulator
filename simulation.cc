#include "./include/simulation.h"

Simulation::Simulation(Dealer* dealer, Player* player, Shoe* shoe) {
    _dealer = dealer;
    _player = player;
    _shoe = shoe;
}

Simulation::~Simulation() {
}

void Simulation::step() {
    _player->bet(2);

    _dealer->dealHand(_player, _shoe);
    auto upCard = _dealer->dealHandToSelf(_shoe);
    _player->count(upCard);

    if(_dealer->has(A)) {
        _player->doInsuranceOrNot(_dealer);
    }
    if(_dealer->hasBlackjack() && !Config::instance()->isNoHoleCard) {
        if(_player->takesInsurance()) {
            _player->getPrize(1.5);
        }
    }
    else {
        _player->doAction(_dealer, _shoe);
    }

    if(_dealer->hasBlackjack() && !Config::instance()->isNoHoleCard) {
        if(_player->takesInsurance()) {
            _player->getPrize(1.5);
        }
    }

    _dealer->doAction(_player, _shoe);

    _player->adjust(_dealer->hand());

    _player->recordResult(_dealer);
    _dealer->recordResult();
}

void Simulation::start() {
    int gameCounter = 0;

    std::cout << "Simulation start" << std::endl;
    while(true) {
        _shoe->shuffle();
        _player->onShuffle();

        while(!_shoe->needsShuffle()) {
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
