/**
 * rule.h
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
