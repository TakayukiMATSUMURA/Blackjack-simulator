#include "./include/player.h"
#include "./include/dealer.h"

Player::Player(int bankroll, IStrategy *strategy)
{
    _bankroll = bankroll;
    _strategy = strategy;

    _totalBetAmount = 0;
    _totalPrizeAmount = 0;

    _resultCounter = new Counter<std::string>();
    _handRankCounter = new Counter<std::string>();
    _handRankAfterDoubleDownCounter = new Counter<std::string>();
    _insuranceCounter = new Counter<bool>();
    _doubledownCounter = new Counter<std::string>();
    _splitCounter = new Counter<int>();
    _cardCountOfHandCounter = new Counter<int>();
    _diffBetweenShufflesCounter = new Counter<double>();
}

Player::~Player()
{
    delete _strategy;
    delete _resultCounter;
    delete _handRankCounter;
    delete _handRankAfterDoubleDownCounter;
    delete _insuranceCounter;
    delete _doubledownCounter;
    delete _splitCounter;
    delete _cardCountOfHandCounter;
    delete _diffBetweenShufflesCounter;
}

void Player::bet(int unit)
{
    _takesInsurance = false;

    int amount = _strategy->betAmount(unit * 2);
    _betAmount = amount;
    _bankroll -= _betAmount;
    _totalBetAmount += _betAmount;

    if (Config::instance()->isDebugMode)
    {
        std::cout << "Player's bankroll:" << (double)(_bankroll + _betAmount) / 2 << std::endl;
        std::cout << "Strategy:" << _strategy->toString() << std::endl;
        std::cout << "Player bets:" << unit << std::endl;
    }
}

int Player::betAmount() const
{
    return _betAmount;
}

void Player::receive(std::vector<Card *> &cards)
{
    auto hand = new Hand(cards, _betAmount);
    for (Card *card : cards)
    {
        count(card);
    }
    _hands.push_back(hand);

    if (Config::instance()->isDebugMode)
    {
        std::cout << "Player's hand:" << hand->toString() << std::endl;
    }
}

void Player::getPrize(float rate, Hand *hand)
{
    if (hand == nullptr)
        hand = _hands[0];

    int prize = hand->bet() * rate;
    _bankroll += prize;
    _totalPrizeAmount += prize;

    if (Config::instance()->isDebugMode)
    {
        std::cout << "Player gets " << prize << " with " << hand->toString() << std::endl;
    }
}

void Player::doInsuranceOrNot(Dealer *dealer)
{
    if (!_strategy->takesInsurance())
        return;

    _takesInsurance = true;
    _bankroll -= _betAmount / 2;
    _totalBetAmount += _betAmount / 2;

    _insuranceCounter->count(dealer->hasBlackjack());
    if (Config::instance()->isDebugMode)
    {
        std::cout << "Player takes insurance(bet:" << (_betAmount / 2) << ")" << std::endl;
    }
}

bool Player::takesInsurance() const
{
    return _takesInsurance;
}

void Player::doAction(Dealer *dealer, Shoe *shoe)
{
    if (hasBlackjack())
        return;
    int handCounter = 0;
    while (handCounter < _hands.size())
    {
        auto currentHand = _hands[handCounter++];
        Action action = Action::Invalid;

        if (Config::instance()->isDebugMode)
        {
            std::cout << "Player's hand:" << currentHand->toString() << std::endl;
        }
        while (action != Action::Stand && action != Action::DoubleDown && action != Action::Surrender && !currentHand->isBusted())
        {
            action = _strategy->getAction(currentHand, dealer->upCardRank());
            if (_hands.size() > 2)
            {
                if (action == Action::Surrender)
                    action = Action::Hit;
                if (action == Action::DoubleDown && !Rule::instance()->DaS)
                    action = Action::Hit;
            }

            if (Config::instance()->isDebugMode && !currentHand->isBusted())
            {
                auto actionString = action == Action::Hit ? "Hit" : action == Action::Surrender ? "Surrender"
                                                                : action == Action::Split       ? "Split"
                                                                : action == Action::DoubleDown  ? "Double down"
                                                                                                : "Stand";
                std::cout << "action:" << actionString << std::endl;
            }

            Card *card;
            switch (action)
            {
            case Action::Hit:
                card = shoe->draw();
                count(card);
                currentHand->add(card);
                break;
            case Action::DoubleDown:
                _bankroll -= currentHand->bet();
                _totalBetAmount += currentHand->bet();
                card = shoe->draw();
                count(card);
                currentHand->doubleDownWith(card);
                _handRankAfterDoubleDownCounter->count(currentHand->rankString());
                break;
            case Action::Split:
            {
                _bankroll -= currentHand->bet();
                _totalBetAmount += currentHand->bet();

                if (currentHand->isPairOf(A))
                {
                    action = Action::Stand;
                    handCounter++;
                }

                auto card0 = shoe->draw();
                count(card0);
                auto card1 = shoe->draw();
                count(card1);
                auto newHand = currentHand->split(card0, card1);
                _hands.push_back(newHand);
                break;
            }
            case Action::Surrender:
                currentHand->surrender();
                getPrize(0.5, currentHand);
                break;
            default:
                break;
            }

            if (Config::instance()->isDebugMode)
            {
                std::cout << "hand:" << currentHand->toString() << std::endl;
            }
        }
    }
}

void Player::adjust(Hand *dealersHand)
{
    if (dealersHand->isBlackjack())
    {
        if (hasBlackjack())
        {
            getPrize(1, _hands[0]);
        }

        return;
    }

    if (hasBlackjack())
    {
        getPrize(2.5, _hands[0]);
        return;
    }

    for (const auto &hand : _hands)
    {
        if (hand->isBusted())
            continue;

        if (hand->winsAgainst(dealersHand))
        {
            getPrize(2, hand);
        }
        else if (!hand->loses(dealersHand))
        {
            getPrize(1, hand);
        }
    }
}

bool Player::hasBlackjack() const
{
    return _hands.size() == 1 && _hands[0]->isBlackjack();
}

void Player::count(Card *card)
{
    _strategy->count(card);
}

void Player::onShuffle()
{
    _strategy->reset();
    _buffer.push_back(_bankroll);

    if (_buffer.size() == Config::instance()->N)
    {
        auto sum = std::accumulate(_buffer.begin(), _buffer.end(), 0);
        auto average = std::round((double)sum / Config::instance()->N) / 2;
        _diffBetweenShufflesCounter->count(average);
        _buffer.clear();
    }

    _bankroll = 0;
}

void Player::recordResult(Dealer *dealer)
{
    if (_hands.size() > 1)
    {
        _splitCounter->count(_hands.size() - 1);
    }

    for (const auto &hand : _hands)
    {
        _cardCountOfHandCounter->count(hand->size());

        if (!dealer->hasBlackjack())
        {
            _handRankCounter->count(hand->rankString());
        }
        std::string result;
        if (hand->loses(dealer->hand()))
        {
            result = "lose";
        }
        else if (hand->winsAgainst(dealer->hand()))
        {
            result = "win";
        }
        else
        {
            result = "tie";
        }
        _resultCounter->count(result);
        if (hand->isDoubledown())
        {
            _doubledownCounter->count(result);
        }
    }
}

void Player::resetHands()
{
    for (const auto &hand : _hands)
    {
        delete hand;
    }

    _hands.clear();
}

float Player::expectedValue(int gameCount) const
{
    return ((float)_bankroll * 100 / 2) / gameCount;
}

std::string Player::toString() const
{
    std::string result = "############# Player ##############\n";
    result += "Result\n" + _resultCounter->toStringInDescendingOrder() + "\n\n";
    result += "Insurance accuracy\n" + _insuranceCounter->getStringPercentageOf(true) + "\n\n";
    result += "Double down\n" + _doubledownCounter->toStringInDescendingOrder() + "\n\n";
    result += "Hand distribution after Double down\n" + _handRankAfterDoubleDownCounter->toStringInDescendingOrder() + "\n\n";

    result += "Split\n" + _splitCounter->toStringInDescendingOrder() + "\n\n";
    result += "Cards per hand\n" + _cardCountOfHandCounter->toStringInDescendingOrder() + "\n\n";
    result += "Hand distribution\n" + _handRankCounter->toStringInDescendingOrder() + "\n\n";
    result += "Bankroll distribution(per shoe)\n" + _diffBetweenShufflesCounter->toStringInDescendingOrder() + "\n\n";
    _diffBetweenShufflesCounter->saveDataTo("data/bankroll_distribution.csv");

    return result;
}
