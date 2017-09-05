/**
 * basicstrategy.h
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

#include "../rule.h"
#include "./istrategy.h"
#include "../lib/csvreader.h"

class BasicStrategy : public IStrategy {
public:
    BasicStrategy() {
        _csvreader = new CSVReader("./strategies/BasicStrategy.csv");
    }
    
    virtual ~BasicStrategy() {
        delete _csvreader;
    }

    void reset() override {}

    void count(Card*) override {}
    
    int getAction(Hand* hand, int dealersUpcardRank) const override {
        std::string dealersUpCard = (dealersUpcardRank == 1) ? "A" : std::to_string(dealersUpcardRank);
        std::string playersHand = "";
        if(hand->isPair()) {
            if(hand->isPairOf(A)) {
                playersHand = "AA";
            }
            else {
                playersHand = std::to_string(hand->rank() / 2) + std::to_string(hand->rank() / 2);
            }
        }
        else if(hand->isSoft() && hand->rank() <= 19) {
            playersHand = "A" + std::to_string(hand->rank() - 11);
        }
        else {
            playersHand = (hand->rank() <= 8) ? "8" :
                (hand->rank() >= 17) ? "17" : std::to_string(hand->rank());
        }
        auto result = _csvreader->getContent(playersHand, dealersUpCard);
        
        if(result == "D") {
            if(hand->canDoubleDownOrSurrender()) {
                return DoubleDown;
            }
            
            return Hit;
        }
        
        if(result == "S") return Stand;
        if(result == "P") return Split;
        if(result == "PH") {
            return Rule::instance()->DaS ? Split : Hit;
        }
        
        if(result == "R") {
            if(hand->canDoubleDownOrSurrender()) {
                return Surrender;
            }
            return Hit;
        }
        return Hit;
    }
    
    bool takesInsurance() const override {
        return false;
    }

    int betAmount(int unit) const override { return unit; }
    
private:
    CSVReader* _csvreader;
};
