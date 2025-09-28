#include <iostream>

#include "./include/dealer.h"
#include "./include/player.h"
#include "./include/rule.h"
#include "./include/simulation.h"
#include "./include/simulation_with_same_hand.h"
#include "./include/strategies/basicstrategy.h"
#include "./include/strategies/KOSmart.h"
#include "./include/lib/optioninterpreter.h"

int main(int argc, char *argv[])
{
    std::cout << "Blackjack simulator" << std::endl;

    auto optionInterpreter = new OptionInterpreter();
    optionInterpreter->add("g", " [n:int]:simulation game", [&](std::string arg)
                           { Config::parameters.game = std::stoi(arg); });
    optionInterpreter->add("d", ":debug mode(display all games)", [&]()
                           { Config::parameters.isDebugMode = true; });
    optionInterpreter->add("detailresult", ":display detail result", [&]()
                           { Config::parameters.displaysDetailResult = true; });
    optionInterpreter->add("deck", " [1|2|6|8|infinite]:deck(default:6)", [&](std::string arg)
                           { Rule::parameters.deck = arg == "infinite" ? INT_MAX : std::stoi(arg); });
    optionInterpreter->add("shoe", ":shoe game(not shuffle machine)", [&]()
                           { Rule::parameters.isShuffleMachine = false; });
    optionInterpreter->add("playershand", ":player's hand(for example T9)", [&](std::string arg)
                           { Config::parameters.playersHand = arg; });
    optionInterpreter->add("playersaction", ":player's action{Hit|Stand|Split|DoubleDown}", [&](std::string arg)
                           { Config::parameters.playersAction = arg; });
    optionInterpreter->add("dealersupcardrank", ":dealer's upcard rank", [&](std::string arg)
                           { Config::parameters.dealersUpCardRank = arg == "T" ? 10 : arg == "A" ? 1
                                                                                                 : std::stoi(arg); });
    optionInterpreter->add("noholecard", ":dealer checks blackjack or not after player's action", [&]()
                           { Config::parameters.isNoHoleCard = true; });
    optionInterpreter->add("hitsoft17", ":dealer hits soft 17", [&]()
                           { Rule::parameters.hitSoft17 = true; });
    optionInterpreter->add("s", " [basic|KORookie|KOSmart]:strategy(default:basic)", [&](std::string arg)
                           { Config::parameters.strategy = arg; });
    optionInterpreter->add("b", " [n:int]:bet spread(default:1)", [&](std::string arg)
                           { Config::parameters.betSpread = std::stoi(arg); });
    optionInterpreter->add("n", " [n:int]:num of sample average(default:1)", [&](std::string arg)
                           { Config::parameters.N = std::stoi(arg); });
    optionInterpreter->analize(argc, argv);

    if (Config::parameters.game <= 0)
    {
        optionInterpreter->showUsages();
        std::cout << "execute with -g option to start simulation" << std::endl;
        exit(0);
    }
    delete optionInterpreter;

    auto strategy = Config::instance()->getStrategy();
    auto player = new Player(0, strategy);
    auto dealer = new Dealer();
    auto shoe = new Shoe(Rule::instance()->deck, Rule::instance()->isShuffleMachine);
    Simulation *simulation;
    if (Config::instance()->parameters.fixCards())
    {
        simulation = new SimulationWithSameHand(dealer, player, shoe, Config::instance()->parameters.playersHand, Config::instance()->parameters.dealersUpCardRank);
    }
    else
    {
        simulation = new Simulation(dealer, player, shoe);
    }
    simulation->start();

    delete player;
    delete dealer;
    delete shoe;
    delete simulation;
    return 0;
}
