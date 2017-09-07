#include <iostream>

#include "./dealer.h"
#include "./player.h"
#include "./rule.h"
#include "./strategies/basicstrategy.h"
#include "./strategies/KOSmart.h"
#include "./lib/optioninterpreter.h"

int main(int argc, char *argv[]) {
    auto dealer = Dealer::instance();
    
    int initialBankroll = 100;
    //auto strategy = new BasicStrategy();
    //auto strategy = new KORookie(Rule::instance()->deck, 10);
    auto strategy = new KOSmart(Rule::instance()->deck, 2);
    auto player = new Player(initialBankroll, strategy);
    
    dealer->add(player);
    
    for(int i = 0; i < 10000; i++) {
        dealer->shuffle();
        
        while(!dealer->needsShuffle()) {
            player->bet(10);
            
            dealer->dealHandTo(player);            
            dealer->dealHandToSelf();
            
            if(dealer->hasAce()) {
                player->doInsuranceOrNot();
            }
            if(dealer->hasBlackjack()) {
                if(player->takesInsurance()) {
                    player->getPrize(1.5);
                }
                if(player->hasBlackjack()) {
                    player->getPrize(1);
                }
            }
            else {
                if(player->hasBlackjack()) {
                    player->getPrize(2.5);
                }
                else {
                    player->doAction();
                }
            }
            dealer->doAction();
            
            player->adjust();
            
            player->recordResult();
            dealer->recordResult();
        }
    }

    std::cout << "Simulation result" << std::endl;
    std::cout << player->toString() << std::endl;
    std::cout << std::endl;
    std::cout << dealer->toString() << std::endl;

    delete player;
    return 0;
}
