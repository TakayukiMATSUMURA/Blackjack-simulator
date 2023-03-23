#include <iostream>

#include "./include/dealer.h"
#include "./include/player.h"
#include "./include/rule.h"
#include "./include/simulation.h"
#include "./include/simulation_with_same_hand.h"
#include "./include/strategies/basicstrategy.h"
#include "./include/strategies/KOSmart.h"
#include "./include/lib/optioninterpreter.h"

int main(int argc, char *argv[]) {
    std::cout << "Blackjack simulator" << std::endl;

    auto optionInterpreter = new OptionInterpreter();
    optionInterpreter->add("g", " [n:int]:simulation game", [&](std::string arg){ Config::parameters.game = std::stoi(arg); });
    optionInterpreter->add("d", ":debug mode(display all games)", [&](){ Config::parameters.isDebugMode = true; });
    optionInterpreter->add("dealerdetail", ":display detail dealer's hand distributions", [&](){ Config::parameters.displaysDealerDetails = true; });
    optionInterpreter->add("deck", " [1|2|6|8|infinite]:deck(default:6)", [&](std::string arg){ Rule::parameters.deck = arg == "infinite" ? INT_MAX : std::stoi(arg); });
    optionInterpreter->add("playershand", ":player's hand(for example T9)", [&](std::string arg){ Config::parameters.playersHand = arg; });
    optionInterpreter->add("dealersupcardrank", ":dealer's upcard rank", [&](std::string arg){ Config::parameters.dealersUpCardRank = arg == "T" ? 10 : arg == "A" ? 1 : std::stoi(arg); });
    optionInterpreter->add("noholecard", ":dealer checks blackjack or not after player's action", [&](){ Config::parameters.isNoHoleCard = true; });
    optionInterpreter->add("s", " [basic|KORookie|KOSmart]:strategy(default:basic)", [&](std::string arg){ Config::parameters.strategy = arg; });
    optionInterpreter->add("b", " [n:int]:bet spread(default:1)", [&](std::string arg){ Config::parameters.betSpread = std::stoi(arg); });
    optionInterpreter->add("n", " [n:int]:num of sample average(default:1)", [&](std::string arg){ Config::parameters.N = std::stoi(arg); });
    optionInterpreter->analize(argc, argv);

    if(Config::parameters.game <= 0) {
        optionInterpreter->showUsages();
        std::cout << "execute with -g option to start simulation" << std::endl;
        exit(0);
    }
    delete optionInterpreter;

    auto strategy = Config::instance()->getStrategy();
    auto player = new Player(0, strategy);
    auto dealer = new Dealer();
    auto shoe = new Shoe(Rule::instance()->deck);
    Simulation* simulation;
    if(Config::instance()->parameters.fixCards()) {
        auto playersHand = Config::instance()->parameters.playersHand;
        auto card0Rank = playersHand[0] == 'T' ? 10 : playersHand[0] == 'A' ? 1 : playersHand[0] - '0';
        auto card1Rank = playersHand[1] == 'T' ? 10 : playersHand[1] == 'A' ? 1 : playersHand[1] - '0';
        auto dealersUpCardRank = Config::instance()->parameters.dealersUpCardRank;
        simulation = new SimulationWithSameHand(dealer, player, shoe, card0Rank, card1Rank, dealersUpCardRank);
    }
    else {
        simulation = new Simulation(dealer, player, shoe);
    }
    simulation->start();

    delete player;
    delete dealer;
    delete shoe;
    delete simulation;
    return 0;
}
