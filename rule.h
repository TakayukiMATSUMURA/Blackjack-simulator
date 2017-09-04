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

#include "./lib/singleton.h"

class Rule : public Singleton<Rule> {
public:
    Rule(int deck, bool DaS) : deck(deck),
                               DaS(DaS),
                               penetration(deck == 1 ? 65 : 75),
                               hitSoft17(deck == 1) {
    }
    virtual ~Rule() {}
    
    const int deck;
    const bool DaS;
    const int penetration;
    const bool hitSoft17;
    
    static Rule* create() {
        static Rule rule(6, false);
        return &rule;
    }
};
