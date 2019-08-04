/**
 * istrategy.h
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

#include <string>

#include "../hand.h"

enum class Action {
    Invalid = -1,
    Hit = 0,
    Stand = 1,
    Surrender = 2,
    Split = 3,
    DoubleDown = 4,
};

class IStrategy {
public:
    IStrategy() {}
    virtual ~IStrategy() {}

    virtual void reset() = 0;
    virtual void count(Card*) = 0;
    virtual Action getAction(Hand*, int) const = 0;
    virtual bool takesInsurance() const = 0;
    virtual int betAmount(int) const = 0;
    virtual std::string toString() const = 0;
};
