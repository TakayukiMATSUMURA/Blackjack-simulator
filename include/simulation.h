/**
 * simulation.h
 *
 * Copyright (c) 2019 Takayuki Matsumura
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

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
