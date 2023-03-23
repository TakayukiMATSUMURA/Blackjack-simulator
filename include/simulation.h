#pragma once

#include "./card.h"
#include "./player.h"
#include "./dealer.h"
#include "./shoe.h"

class Simulation {
public:
    Simulation(Dealer *, Player *, Shoe *);
    virtual ~Simulation();
    virtual void start();

protected:
    virtual void dealHandToPlayer();
    virtual void dealHandToDealer();
    void step();
    void showResult();
    Dealer *_dealer;
    Player* _player;
    Shoe* _shoe;
};
