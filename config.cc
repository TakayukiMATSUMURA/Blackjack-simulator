/**
 * config.cc
 *
 * Copyright (C) 2017 Takayuki MATSUMURA
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * See LICENSE file on the base directory for more information.
 *
 */
/**********************************************************************/
#include "./config.h"

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
