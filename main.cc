#include <iostream>

#include "./dealer.h"
#include "./player.h"
#include "./rule.h"
#include "./strategies/basicstrategy.h"
#include "./strategies/KOSmart.h"
#include "./lib/optioninterpreter.h"

int main(int argc, char *argv[]) {
    auto optionInterpreter = new OptionInterpreter();
    optionInterpreter->add("debug", ":debug mode(display all games)", [&](){
            Config::parameters.isDebugMode = true;
        });
    optionInterpreter->add("d", ":deck", [&](std::string arg){
            Rule::parameters.deck = std::stoi(arg);
        });
    optionInterpreter->add("s", ":strategy(basic or KORookie or KOSmart)", [&](std::string arg){
            Config::parameters.strategy = arg;
        });
    optionInterpreter->add("b", ":bet spread", [&](std::string arg){
            Config::parameters.betSpread = std::stoi(arg);
        });
    optionInterpreter->add("g", ":game", [&](std::string arg){
            Config::parameters.game = std::stoi(arg);
        });
    optionInterpreter->analize(argc, argv);
    delete optionInterpreter;
    
    auto dealer = Dealer::instance();
    
    int initialBankroll = 100;
    auto strategy = Config::instance()->getStrategy();
    auto player = new Player(initialBankroll, strategy);
    
    dealer->add(player);
    int gameCounter = 0;
    while(true) {
        dealer->shuffle();
        
        while(!dealer->needsShuffle()) {
            if(Config::instance()->isDebugMode) {
                std::cout << "Game:" << gameCounter << " start" << std::endl;
            }
            
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
    
    std::cout << "Simulation result" << std::endl;
    std::cout << player->toString() << std::endl;
    std::cout << std::endl;
    std::cout << dealer->toString() << std::endl;

    delete player;
    return 0;
}
