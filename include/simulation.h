#pragma once

#include "./card.h"
#include "./player.h"
#include "./dealer.h"
#include "./shoe.h"

class Simulation {
public:
    Simulation(Dealer*, Player*, Shoe*);
    virtual ~Simulation();
    void start();

private:
    Dealer* _dealer;
    Player* _player;
    Shoe* _shoe;

    void step();
};
