#pragma once

#include "../rule.h"
#include "./istrategy.h"
#include "../lib/csvreader.h"

class BasicStrategy : public IStrategy {
public:
    BasicStrategy() {
        _csvreader = new CSVReader("./include/strategies/BasicStrategy.csv");
    }

    virtual ~BasicStrategy() {
        delete _csvreader;
    }

    void reset() override {}

    void count(Card*) override {}

    Action getAction(Hand* hand, int dealersUpcardRank) const override {
        std::string dealersUpCard = (dealersUpcardRank == 1) ? "A" : std::to_string(dealersUpcardRank);
        std::string playersHand = "";
        if(hand->isPair()) {
            playersHand = hand->isPairOf(A) ? "AA" : std::to_string(hand->rank() / 2) + std::to_string(hand->rank() / 2);
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
            return hand->canDoubleDownOrSurrender() ? Action::DoubleDown : Action::Hit;
        }

        if(result == "S") return Action::Stand;
        if(result == "P") return Action::Split;
        if(result == "PH") {
            return Rule::instance()->DaS ? Action::Split : Action::Hit;
        }

        if(result == "R") {
            return hand->canDoubleDownOrSurrender() ? Action::Surrender : Action::Hit;
        }

        return Action::Hit;
    }

    bool takesInsurance() const override {
        return false;
    }

    int betAmount(int unit) const override { return unit; }

    std::string toString() const override { return "Basic strategy"; }

private:
    CSVReader* _csvreader;
};
