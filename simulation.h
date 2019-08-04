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
    Simulation();
    virtual ~Simulation();
                                       
    static void start(Player*, int);
    
protected:

private:

};
