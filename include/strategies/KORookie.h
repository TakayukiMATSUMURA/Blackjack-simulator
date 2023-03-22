#pragma once

#include "./basicstrategy.h"

class KORookie : public IStrategy {
public:
    KORookie(int deck, int spread) : initialRunningCount(4 - 4 * deck),
                                        keyCount(deck == 2 ? 1 : deck == 6 ? -4 : deck == 8 ? -6 : 2),
                                        _basicStrategy(new BasicStrategy()),
                                        _spread(spread)
    {
        reset();
    }
    virtual ~KORookie() {
        delete _basicStrategy;
    }

    void reset() override {
        _runningCount = initialRunningCount;
    }

    void count(Card* card) override {
        auto rank = card->rank;
        if(rank == A || rank == T) _runningCount--;
        else if(rank >= 2 && rank <= 7) _runningCount++;
    }

    Action getAction(Hand* hand, int dealersUpcardRank) const override {
        return _basicStrategy->getAction(hand, dealersUpcardRank);
    }

    bool takesInsurance() const override {
        return false;
    }

    int betAmount(int unit) const override {
        return _runningCount >= keyCount ? unit * _spread : unit;
    }

    std::string toString() const override {
        std::string result = "KO rookie";
        result += "(key count:" + std::to_string(keyCount);
        result += " running count:" + std::to_string(_runningCount) + ")";
        return result;
    }

    const int initialRunningCount;
    const int keyCount;

protected:
    const IStrategy* _basicStrategy;
    int _runningCount;

private:
    const int _spread;
};
