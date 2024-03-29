#include "./include/config.h"

Config::Parameters Config::parameters;

Config::Config(Parameters params) : isDebugMode(params.isDebugMode),
                                    displaysDetailResult(params.displaysDetailResult),
                                    strategy(params.strategy),
                                    betSpread(params.betSpread),
                                    game(params.game),
                                    N(params.N),
                                    isNoHoleCard(params.isNoHoleCard),
                                    playersHand(params.playersHand),
                                    playersAction(params.playersAction),
                                    dealersUpCardRank(params.dealersUpCardRank)
{
    std::cout << "Config" << std::endl;
    if (isDebugMode)
    {
        std::cout << "debug mode" << std::endl;
    }
    if (playersAction != "")
        std::cout << "strategy:" << playersAction << std::endl;
    else
        std::cout << "strategy:" << strategy << std::endl;

    if (strategy != "basic")
    {
        std::cout << "bet spread:" << betSpread << std::endl;
    }
    if (isNoHoleCard)
    {
        std::cout << "No hole card" << std::endl;
    }
    if (playersAction != "")
    {
        std::cout << "Player's Action:" << playersAction << std::endl;
    }
    std::cout << "game:" << game << std::endl;
    std::cout << "num for sample average:" << N << std::endl;
    std::cout << std::endl;
}

Config::~Config()
{
}

IStrategy *Config::getStrategy()
{
    if (strategy == "KORookie")
    {
        return new KORookie(Rule::instance()->deck, betSpread);
    }
    else if (strategy == "KOSmart")
    {
        return new KOSmart(Rule::instance()->deck, betSpread);
    }
    else if (playersAction != "")
    {
        return new BasicStrategyBuild(playersHand, playersAction);
    }

    return new BasicStrategy();
}
