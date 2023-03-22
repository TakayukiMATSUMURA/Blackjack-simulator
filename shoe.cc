#include "./include/shoe.h"

Shoe::Shoe(int deckNum) {
    _cards = Card::getDeck(deckNum);
}

Shoe::~Shoe() {
}

void Shoe::shuffle() {
    _cards = Card::getDeck(Rule::instance()->deck);

    std::random_device rnd;
    std::mt19937_64 mt(rnd());
    std::shuffle(std::begin(_cards), std::end(_cards), mt);
}

bool Shoe::needsShuffle() const {
    return _cards.size() <= Rule::instance()->deck * 52 * (100.0 - Rule::instance()->penetration) / 100;
}

Card* Shoe::draw() {
    auto card = _cards.back();
    _cards.pop_back();
    return card;
}
