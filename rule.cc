/**
 * rule.cc
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
#include "./rule.h"

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


