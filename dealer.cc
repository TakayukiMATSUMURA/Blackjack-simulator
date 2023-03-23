#include "./include/dealer.h"

Dealer::Dealer() {
    _hand = nullptr;
    _handRankCounter = new Counter<std::string>();

    for(int i = 0; i < 10; i++) {
        _handRankCounterForEachRanks[i] = new Counter<std::string>();
    }

    _allHandCardCounter = new Counter<int>();
    _patHandCardCounter = new Counter<int>();
    _bustedHandCardCounter = new Counter<int>();
}

Dealer::~Dealer() {
    delete _hand;
    delete _handRankCounter;

    for(const auto& counter : _handRankCounterForEachRanks) {
        delete counter;
    }

    delete _allHandCardCounter;
    delete _patHandCardCounter;
    delete _bustedHandCardCounter;
}

void Dealer::dealHand(Player* player, Shoe* shoe) {
    std::vector<Card*> cards;
    for(int i = 0; i < 2; i++) {
        cards.push_back(shoe->draw());
    }

    player->receive(cards);
}

Card* Dealer::dealHandToSelf(Shoe* shoe) {
    delete _hand;

    std::vector<Card*> cards;
    _upCard = shoe->draw();
    cards.push_back(_upCard);
    _holeCard = shoe->draw();
    cards.push_back(_holeCard);

    _hand = new Hand(cards);

    if(Config::instance()->isDebugMode) {
        std::cout << "Dealer's upcard:" << _upCard->toString() << std::endl;
    }

    return _upCard;
}

bool Dealer::has(int rank) const {
    return _upCard->rank == rank;
}

bool Dealer::hasBlackjack() const {
    return _hand->isBlackjack();
}

bool Dealer::isBusted() const {
    return _hand->isBusted();
}

void Dealer::recordResult() {
    _handRankCounter->count(_hand->rankString());
    auto index = _upCard->rank == A ? 9 : _upCard->rank - 2;
    _handRankCounterForEachRanks[index]->count(_hand->rankString());

    _allHandCardCounter->count(_hand->size());
    if(isBusted()) {
        _bustedHandCardCounter->count(_hand->size());
    }
    else {
        _patHandCardCounter->count(_hand->size());
    }
}

void Dealer::doAction(Player* player, Shoe* shoe) {
    player->count(_holeCard);

    while(_hand->rank() < 17 || (_hand->isSoft(17) && Rule::instance()->hitSoft17)) {
        auto card = shoe->draw();
        player->count(card);
        _hand->add(card);
    }

    if(Config::instance()->isDebugMode) {
        std::cout << "Dealer's hand:" << _hand->toString() << std::endl;
    }
}

int Dealer::upCardRank() const {
    return _upCard->rank;
}

std::string Dealer::toString() const {
    std::string result = "############# Dealer ##############\n";
    if(!Config::instance()->displaysDealerDetails) {
        result += "Hand distribution\n" + _handRankCounter->toStringInDescendingOrder() + "\n\n";
    }
    else {
        for(int i = 0; i < 10; i++) {
            auto upCard = (i == 9 ? "A" : std::to_string(i + 2));
            result += "Hand distribution(upcard:" + upCard + ")\n";
            result += _handRankCounterForEachRanks[i]->toStringInDescendingOrder() + "\n\n";
        }

        result += "Hand distribution(all)\n" + _handRankCounter->toStringInDescendingOrder() + "\n\n";
        result += "Hand distribution except for BJ\n";
        for(int i = 0; i < 2; i++) {
            auto upCard = i == 0 ? std::to_string(10) : "A";
            result += "upcard:" + upCard + "\n";
            auto index = i + 8;
            _handRankCounterForEachRanks[index]->erase("BJ");
            result += _handRankCounterForEachRanks[index]->toStringInDescendingOrder() + "\n\n";
        }
        _handRankCounter->erase("BJ");
        result += "all\n" + _handRankCounter->toStringInDescendingOrder() + "\n\n";
    }

    result += "Hand cards distribution\n";
    result += _allHandCardCounter->toStringInDescendingOrder() + "\n\n";

    result += "Hand cards distribution(Pat hand)\n";
    result += _patHandCardCounter->toStringInDescendingOrder() + "\n\n";

    result += "Hand cards distribution(Busted hand)\n";
    result += _bustedHandCardCounter->toStringInDescendingOrder() + "\n\n";

    return result;
}
