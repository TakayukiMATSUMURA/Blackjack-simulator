#include <iostream>

#include "./dealer.h"
#include "./player.h"
#include "./rule.h"
#include "./simulation.h"
#include "./strategies/basicstrategy.h"
#include "./strategies/KOSmart.h"
#include "./lib/optioninterpreter.h"

int main(int argc, char *argv[]) {
    std::cout << "Blackjack simulator" << std::endl;
    
    auto optionInterpreter = new OptionInterpreter();
    optionInterpreter->add("g", " [n:int]:simulation game",
                           [&](std::string arg){
                               Config::parameters.game = std::stoi(arg);
                           }
    );
    optionInterpreter->add("d", ":debug mode(display all games)",
                           [&](){
                               Config::parameters.isDebugMode = true;
                           }
    );
    optionInterpreter->add("dealerdetail", ":display detail dealer's hand distributions",
                           [&](){
                               Config::parameters.displaysDealerDetails = true;
                           }
    );
    optionInterpreter->add("deck", " [1|2|6|8]:deck(default:6)",
                           [&](std::string arg){
                               Rule::parameters.deck = std::stoi(arg);
                           }
    );
    optionInterpreter->add("s", " [basic|KORookie|KOSmart]:strategy(default:basic)",
                           [&](std::string arg){
                               Config::parameters.strategy = arg;
                           }
    );
    optionInterpreter->add("b", " [n:int]:bet spread(default:1)",
                           [&](std::string arg){
                               Config::parameters.betSpread = std::stoi(arg);
                           }
    );
    optionInterpreter->analize(argc, argv);
    
    if(Config::parameters.game <= 0) {
        optionInterpreter->showUsages();
        std::cout << "execute with -g option to start simulation" << std::endl;
        exit(0);
    }
    delete optionInterpreter;
    
    auto strategy = Config::instance()->getStrategy();
    auto player = new Player(0, strategy);
    Simulation::start(player, Config::instance()->game);

    delete player;
    return 0;
}
