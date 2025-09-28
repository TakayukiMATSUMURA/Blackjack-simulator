#pragma once

#include <iostream>

#include "./lib/singleton.h"

class Rule : public Singleton<Rule>
{
public:
    virtual ~Rule();

    const int deck;
    const bool isShuffleMachine;
    const bool DaS;
    const int penetration;
    const bool hitSoft17;

    struct Parameters
    {
        bool DaS = false;
        int deck = 6;
        bool isShuffleMachine = true;
        bool hitSoft17 = false;
    };

    static Parameters parameters;

    static Rule *create()
    {
        static Rule rule(parameters);
        return &rule;
    }

private:
    Rule(Parameters);
};
