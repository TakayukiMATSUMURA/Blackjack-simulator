#pragma once

#include <vector>
#include <string>
#include <numeric>

#include "./card.h"

class Hand
{
public:
    Hand(Card *, int = 0);
    Hand(std::vector<Card *> &, int = 0);
    virtual ~Hand();

    int rank() const;
    void add(Card *);
    bool isBlackjack() const;
    bool isBusted() const;
    bool isSoft() const;
    bool isSoft(int) const;
    bool isPair() const;
    bool isPairOf(int) const;
    bool isDoubledown() const;
    bool canDoubleDownOrSurrender() const;
    void doubleDownWith(Card *);
    Hand *split(Card *, Card *);
    void surrender();
    bool isSurrendered() const;
    int bet() const;

    bool winsAgainst(Hand *) const;
    bool loses(Hand *) const;

    std::string rankString() const;
    std::string toString() const;
    std::string toStringCards() const;

    int size() const { return _cards.size(); }

private:
    int sum() const;

    std::vector<Card *> _cards;
    int _bet;

    bool _isDoubledown;
    bool _isSurrendered;
};
