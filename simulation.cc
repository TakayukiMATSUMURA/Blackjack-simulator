/**
 * simulation.cc
 *
 * Copyright (c) 2019 Takayuki Matsumura
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include "simulation.h"

void Simulation::start(Player* player, int times) {
    auto dealer = Dealer::instance();
    dealer->add(player);
    
    int gameCounter = 0;
    
    std::cout << "Simulation start" << std::endl;
    while(true) {
        dealer->shuffle();
        
        while(!dealer->needsShuffle()) {
            if(Config::instance()->isDebugMode) {
                std::cout << "Game:" << gameCounter << " start" << std::endl;
            }
            
            player->bet(2);
            
            dealer->dealHandTo(player);            
            dealer->dealHandToSelf();
            
            if(dealer->has(A)) {
                player->doInsuranceOrNot();
            }
            if(dealer->hasBlackjack()) {
                if(player->takesInsurance()) {
                    player->getPrize(1.5);
                }
            }
            else {
                player->doAction();
            }
            dealer->doAction();
            
            player->adjust();
            
            player->recordResult();
            dealer->recordResult();
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
    std::cout << player->toString() << std::endl;
    std::cout << std::endl;
    std::cout << dealer->toString() << std::endl;
    
}
