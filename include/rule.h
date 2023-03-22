#pragma once

#include <iostream>

#include "./lib/singleton.h"

class Rule : public Singleton<Rule> {
public:
    virtual ~Rule();

    const int deck;
    const bool DaS;
    const int penetration;
    const bool hitSoft17;

    struct Parameters {
        bool DaS = false;
        int deck = 6;
    };

    static Parameters parameters;

    static Rule* create() {
        static Rule rule(parameters);
        return &rule;
    }

private:
    Rule(Parameters);
};
