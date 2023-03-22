/**
 * KOSmart.h
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

#include "./KORookie.h"
#include "../lib/csvreader.h"

class KOSmart : public KORookie {
public:
    KOSmart(int deck, int spread) : KORookie(deck, spread) {
        _csvreader = new CSVReader("./strategies/KOSmart.csv");
    }
    virtual ~KOSmart() {
    }

    Action getAction(Hand* hand, int dealersUpcardRank) const override {
        if(!hand->isSoft()) {
            std::string dealersUpCard = (dealersUpcardRank == 1) ? "A" : std::to_string(dealersUpcardRank);
            std::string playersHand = (hand->rank() <= 8) ? "8" :
                (hand->rank() >= 17) ? "17" : std::to_string(hand->rank());

            auto result = _csvreader->getContent(playersHand, dealersUpCard);

            if(hand->rank() >= 14) {
                if(result == "A" && _runningCount >= 4) {
                    return Action::Surrender;
                }
            }
            else if(hand->rank() >= 12) {
                if(result == "A") return _runningCount >= 4 ? Action::Stand : Action::Hit;
                if(result == "B") return _runningCount >= initialRunningCount ? Action::Stand : Action::Hit;
                if(result == "C") {
                    if(Rule::instance()->deck == 1) {
                        return _runningCount >= 0 ? Action::Stand : Action::Hit;
                    }
                    if(Rule::instance()->deck == 2) {
                        return _runningCount >= -4 ? Action::Stand : Action::Hit;
                    }
                }
            }
            else {
                if(result == "A") return _runningCount >= 4 ? Action::DoubleDown : Action::Hit;
                if(result == "B") return _runningCount >= initialRunningCount ? Action::DoubleDown : Action::Hit;
                if(result == "C") {
                    if(Rule::instance()->deck == 1) {
                        return _runningCount >= 0 ? Action::DoubleDown : Action::Hit;
                    }
                    if(Rule::instance()->deck == 2) {
                        return _runningCount >= -4 ? Action::DoubleDown : Action::Hit;
                    }
                }
            }
        }
        return _basicStrategy->getAction(hand, dealersUpcardRank);
    }

    bool takesInsurance() const override {
        return _runningCount >= 3;
    }

    std::string toString() const override {
        std::string result = "KO smart";
        result += "(key count:" + std::to_string(keyCount);
        result += " running count:" + std::to_string(_runningCount) + ")";
        return result;
    }

private:
    CSVReader *_csvreader;
};
