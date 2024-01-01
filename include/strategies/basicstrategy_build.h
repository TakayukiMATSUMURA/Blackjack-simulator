#pragma once

#include "../rule.h"
#include "./istrategy.h"
#include "../lib/csvreader.h"

class BasicStrategyBuild : public IStrategy
{
public:
	BasicStrategyBuild(std::string action) : _action(action) {}
	virtual ~BasicStrategyBuild() {}

	void reset() override {}

	void count(Card *) override {}

	Action getAction(Hand *hand, int dealersUpcardRank) const override
	{
		if (dealersUpcardRank == 1)
			dealersUpcardRank += 10;
		if (!hand->isSoft())
		{
			if (hand->rank() >= 17)
				return Action::Stand;
			if (hand->rank() >= 14)
			{
				return dealersUpcardRank >= 7 ? Action::Hit : Action::Stand;
			}
			if (hand->rank() == 13)
			{
				return dealersUpcardRank >= 7 || dealersUpcardRank == 2 ? Action::Hit : Action::Stand;
			}
			if (hand->rank() == 12)
			{
				return dealersUpcardRank != 5 ? Action::Hit : Action::Stand;
			}
		}

		return _action == "Hit" ? Action::Hit : _action == "Stand"	  ? Action::Stand
											: _action == "Split"	  ? Action::Split
											: _action == "DoubleDown" ? Action::DoubleDown
																	  : Action::Invalid;
	}

	bool takesInsurance() const override { return false; }

	int betAmount(int unit) const override { return unit; }

	std::string toString() const override { return "strategy build"; }

private:
	const std::string _action;
};
