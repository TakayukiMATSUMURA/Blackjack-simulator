/**
 * config.h
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
#pragma once

#include <iostream>

#include "./rule.h"
#include "./strategies/istrategy.h"
#include "./strategies/basicstrategy.h"
#include "./strategies/KORookie.h"
#include "./strategies/KOSmart.h"
#include "./lib/singleton.h"

class Config : public Singleton<Config> {
public:
    virtual ~Config();
    
    static Config* create() {
        static Config instance(parameters);
        return &instance;
    }
    
    const bool isDebugMode;
    const std::string strategy;
    const int betSpread;
    const int game;
    
    IStrategy* getStrategy();
    
    struct Parameters {
        bool isDebugMode = false;
        std::string strategy = "basic";
        int betSpread = 1;
        int game = 10000;
    };
    
    static Parameters parameters;
protected:

private:
    Config(Parameters);
};

