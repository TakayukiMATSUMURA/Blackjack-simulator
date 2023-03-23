#include "./include/shoe.h"

Shoe::Shoe(int deckNum) : _deckNum(deckNum) {
    _cards = Card::getDeck(hasInfiniteDeck() ? 1 : _deckNum);
}

Shoe::~Shoe() {
}

void Shoe::shuffle() {
    _cards = Card::getDeck(hasInfiniteDeck() ? 1 : _deckNum);

    std::random_device rnd;
    std::mt19937_64 mt(rnd());
    std::shuffle(std::begin(_cards), std::end(_cards), mt);
}

bool Shoe::needsShuffle() const {
    if(hasInfiniteDeck()) return false;
    return _cards.size() <= _deckNum * 52 * (100.0 - Rule::instance()->penetration) / 100;
}

bool Shoe::hasInfiniteDeck() const {
    return _deckNum == INT_MAX;
}

Card* Shoe::draw() {
    if(hasInfiniteDeck()) {
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_int_distribution<int> rand(0, _cards.size() - 1);
        return _cards[rand(mt)];
    }

    auto card = _cards.back();
    _cards.pop_back();
    return card;
}

Card* Shoe::pickup(int rank) {
    auto it = std::find_if(std::begin(_cards), std::end(_cards), [rank](Card* card){ return card->rank == rank; });
    auto result = *it;
    if (!hasInfiniteDeck()) {
        _cards.erase(it);
    }

    return result;
}