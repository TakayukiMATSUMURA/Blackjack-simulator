#include "./include/rule.h"
#include "./include/config.h"

Rule::Parameters Rule::parameters;

Rule::Rule(Parameters params) : deck(params.deck),
                                isShuffleMachine(params.isShuffleMachine),
                                DaS(params.DaS),
                                penetration(params.deck == 1 ? 65 : 75),
                                hitSoft17(params.hitSoft17)
{
    std::cout << "Rule" << std::endl;
    auto isInfiniteDeck = deck == INT_MAX;
    std::cout << "deck:" << (isInfiniteDeck ? "infinite" : std::to_string(deck)) << std::endl;
    if (DaS)
    {
        std::cout << "DaS" << std::endl;
    }
    if (!(isInfiniteDeck || Config::parameters.fixCards()))
    {
        std::cout << "penetration:" << penetration << "%" << std::endl;
    }
    if (hitSoft17)
    {
        std::cout << "Dealer hits soft 17" << std::endl;
    }
    else
    {
        std::cout << "Dealer stands all 17s" << std::endl;
    }
    std::cout << std::endl;
}

Rule::~Rule()
{
}
