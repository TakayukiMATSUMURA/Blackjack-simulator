#include "./include/rule.h"

Rule::Parameters Rule::parameters;

Rule::Rule(Parameters params) : deck(params.deck),
                                DaS(params.DaS),
                                penetration(params.deck == 1 ? 65 : 75),
                                hitSoft17(params.deck == 1) {
    std::cout << "Rule" << std::endl;
    std::cout << "deck:" << deck << std::endl;
    if(DaS) {
        std::cout << "DaS" << std::endl;
    }
    std::cout << "penetration:" << penetration << "%" << std::endl;
    if(hitSoft17) {
        std::cout << "dealer hits soft 17" << std::endl;
    }
    std::cout << std::endl;
}

Rule::~Rule() {
}
