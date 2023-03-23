#pragma once

#include "./simulation.h"

class SimulationWithSameHand : public Simulation {
public:
    SimulationWithSameHand(Dealer*, Player*, Shoe*, int, int, int);
    virtual ~SimulationWithSameHand();
    virtual void start() override;

protected:
    virtual void dealHandToPlayer() override;
    virtual void dealHandToDealer() override;

private:
    const int _card0Rank;
    const int _card1Rank;
    const int _dealersUpCardRank;
};
