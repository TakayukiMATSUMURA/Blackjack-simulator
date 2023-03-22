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
