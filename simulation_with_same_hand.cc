#include "./include/simulation_with_same_hand.h"

SimulationWithSameHand::SimulationWithSameHand(Dealer* dealer, Player* player, Shoe* shoe, int card0Rank = -1, int card1Rank = -1, int dealersUpCardRank = -1) : Simulation(dealer, player, shoe), _card0Rank(card0Rank), _card1Rank(card1Rank), _dealersUpCardRank(dealersUpCardRank) {
}

SimulationWithSameHand::~SimulationWithSameHand() {
}

void SimulationWithSameHand::dealHandToPlayer() {
    std::vector<Card*> cards;

    if(_card0Rank != -1) {
        cards.push_back(_shoe->pickup(_card0Rank));
    }
    if(_card1Rank != -1) {
        cards.push_back(_shoe->pickup(_card1Rank));
    }

    for(int i = 0; i < 2 - cards.size(); i++) {
        cards.push_back(_shoe->draw());
    }
    _player->receive(cards);
}

void SimulationWithSameHand::dealHandToDealer() {
    auto upCard = _dealersUpCardRank != -1 ? _shoe->pickup(_dealersUpCardRank) : _shoe->draw();
    auto holeCard = Config::instance()->parameters.isNoHoleCard ? nullptr : _shoe->draw();
    _dealer->dealHandToSelf(upCard, holeCard);
    _player->count(upCard);
}

void SimulationWithSameHand::start() {
    int gameCounter = 0;

    std::cout << "Simulation start" << std::endl;
    while(gameCounter++ < Config::instance()->game) {
        _shoe->shuffle();
        if(Config::instance()->isDebugMode) {
            std::cout << "Game:" << gameCounter << " start" << std::endl;
        }

        step();

        if(Config::instance()->isDebugMode) {
            std::cout << std::endl;
        }
    }

    showResult();
}
