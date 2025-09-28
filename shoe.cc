#include "./include/shoe.h"

Shoe::Shoe(int deckNum, bool isShuffleMachine) : _deckNum(deckNum)
{
    _cards = Card::getDeck(hasInfiniteDeck() ? 1 : _deckNum);
    _isShuffleMachine = isShuffleMachine;
    std::random_device rnd;
    _mt = new std::mt19937(rnd());
}

Shoe::~Shoe()
{
    delete _mt;
}

void Shoe::shuffle()
{
    _cards = Card::getDeck(hasInfiniteDeck() ? 1 : _deckNum);
}

bool Shoe::needsShuffle() const
{
    if (_isShuffleMachine)
        return true;

    if (hasInfiniteDeck())
        return false;
    return _cards.size() <= _deckNum * 52 * (100.0 - Rule::instance()->penetration) / 100;
}

bool Shoe::hasInfiniteDeck() const
{
    return _deckNum == INT_MAX;
}

Card *Shoe::draw()
{
    std::uniform_int_distribution<int> rand(0, _cards.size() - 1);
    auto index = rand(*_mt);

    if (!hasInfiniteDeck())
    {
        _cards.erase(_cards.begin() + index);
    }

    return _cards[index];
}

Card *Shoe::pickup(int rank)
{
    auto it = std::find_if(std::begin(_cards), std::end(_cards), [rank](Card *card)
                           { return card->rank == rank; });
    auto result = *it;
    if (!hasInfiniteDeck())
    {
        _cards.erase(it);
        std::shuffle(std::begin(_cards), std::end(_cards), *_mt);
    }

    return result;
}
