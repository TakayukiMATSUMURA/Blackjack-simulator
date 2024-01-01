#pragma once

#include "./simulation.h"

class SimulationWithSameHand : public Simulation
{
public:
    SimulationWithSameHand(Dealer *, Player *, Shoe *, std::string, int);
    virtual ~SimulationWithSameHand();
    virtual void start() override;

protected:
    virtual void dealHandToPlayer() override;
    virtual void dealHandToDealer() override;

private:
    const std::string _playersHand;
    const int _dealersUpCardRank;
};
