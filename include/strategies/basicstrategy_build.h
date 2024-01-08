#pragma once

#include "../rule.h"
#include "./istrategy.h"
#include "./basicstrategy.h"
#include "../lib/csvreader.h"

class BasicStrategyBuild : public BasicStrategy
{
public:
	BasicStrategyBuild(std::string hand, std::string action) : _hand(hand), _action(action) {}
	virtual ~BasicStrategyBuild() {}

	Action getAction(Hand *hand, int dealersUpcardRank) const override
	{
		if (hand->toStringCards() == _hand)
		{
			return _action == "Hit" ? Action::Hit : _action == "Stand"	  ? Action::Stand
												: _action == "Split"	  ? Action::Split
												: _action == "DoubleDown" ? Action::DoubleDown
												: _action == "Surrender"  ? Action::Surrender
																		  : Action::Invalid;
		}

		return BasicStrategy::getAction(hand, dealersUpcardRank);
	}

	std::string toString() const override { return "strategy build"; }

private:
	const std::string _hand;
	const std::string _action;
};
