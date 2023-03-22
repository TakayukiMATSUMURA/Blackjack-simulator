#include "./include/config.h"

Config::Parameters Config::parameters;

Config::Config(Parameters params) : isDebugMode(params.isDebugMode),
                                    displaysDealerDetails(params.displaysDealerDetails),
                                    strategy(params.strategy),
                                    betSpread(params.betSpread),
                                    game(params.game),
                                    N(params.N)
{
    std::cout << "Config" << std::endl;
    if(isDebugMode) {
        std::cout << "debug mode" << std::endl;
    }
    std::cout << "strategy:" << strategy << std::endl;
    if(strategy != "basic") {
        std::cout << "bet spread:" << betSpread << std::endl;
    }
    std::cout << "game:" << game << std::endl;
    std::cout << "num for sample average:" << N << std::endl;
    std::cout << std::endl;
}

Config::~Config() {
}

IStrategy* Config::getStrategy() {
    if(strategy == "KORookie") {
        return new KORookie(Rule::instance()->deck, betSpread);
    }
    else if(strategy == "KOSmart") {
        return new KOSmart(Rule::instance()->deck, betSpread);
    }

    return new BasicStrategy();
}
