#pragma once

#include <iostream>

#include "./rule.h"
#include "./strategies/istrategy.h"
#include "./strategies/basicstrategy.h"
#include "./strategies/basicstrategy_build.h"
#include "./strategies/KORookie.h"
#include "./strategies/KOSmart.h"
#include "./lib/singleton.h"

class Config : public Singleton<Config>
{
public:
    virtual ~Config();

    static Config *create()
    {
        static Config instance(parameters);
        return &instance;
    }

    const bool isDebugMode;
    const bool displaysDetailResult;
    const std::string strategy;
    const int betSpread;
    const int game;
    const int N;
    const bool isNoHoleCard;
    const std::string playersHand;
    const std::string playersAction;
    const int dealersUpCardRank;

    IStrategy *getStrategy();

    struct Parameters
    {
        bool isDebugMode = false;
        bool displaysDetailResult = false;
        std::string strategy = "basic";
        int betSpread = 1;
        int game = 0;
        int N = 1;
        bool isNoHoleCard = false;
        std::string playersHand = "";
        std::string playersAction = "";
        int dealersUpCardRank = -1;

        bool fixCards() const
        {
            return playersHand != "" || dealersUpCardRank != -1;
        }
    };

    static Parameters parameters;

private:
    Config(Parameters);
};
