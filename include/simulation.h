#pragma once

#include "./card.h"
#include "./player.h"
#include "./dealer.h"

class Simulation {
public:
    Simulation(Dealer*, Player*);
    virtual ~Simulation();
    void start();

private:
    Dealer* _dealer;
    Player* _player;

    void step();
};
