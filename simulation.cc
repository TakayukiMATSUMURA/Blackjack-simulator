#include "./include/simulation.h"

Simulation::Simulation(Dealer *dealer, Player *player, Shoe *shoe)
{
    _dealer = dealer;
    _player = player;
    _shoe = shoe;
}

Simulation::~Simulation()
{
}

void Simulation::dealHandToPlayer()
{
    std::vector<Card *> cards;

    for (int i = 0; i < 2; i++)
    {
        cards.push_back(_shoe->draw());
    }
    _player->receive(cards);
}

void Simulation::dealHandToDealer()
{
    auto upCard = _shoe->draw();
    auto holeCard = Config::instance()->parameters.isNoHoleCard ? nullptr : _shoe->draw();
    _dealer->dealHandToSelf(upCard, holeCard);
    _player->count(upCard);
}

bool Simulation::step()
{
    _player->bet(1);

    dealHandToPlayer();
    dealHandToDealer();

    if (_dealer->has(A))
    {
        _player->doInsuranceOrNot(_dealer);
    }

    // プレイヤーのアクションを固定している時はディーラーのブラックジャックをスキップする
    if (_dealer->hasBlackjack() && Config::instance()->playersAction != "")
    {
        _dealer->resetHand();

        _player->getPrize(1);
        _player->resetHands();
        return false;
    }

    if (_dealer->hasBlackjack() && !Config::instance()->isNoHoleCard)
    {
        if (_player->takesInsurance())
        {
            _player->getPrize(1.5);
        }
    }
    else
    {
        _player->doAction(_dealer, _shoe);
    }

    if (_dealer->hasBlackjack() && !Config::instance()->isNoHoleCard)
    {
        if (_player->takesInsurance())
        {
            _player->getPrize(1.5);
        }
    }

    _dealer->doAction(_player, _shoe);

    _player->adjust(_dealer->hand());

    _player->recordResult(_dealer);
    _dealer->recordResult();

    _player->resetHands();
    _dealer->resetHand();

    return true;
}

void Simulation::start()
{
    int gameCounter = 0;

    std::cout << "Simulation start" << std::endl;
    while (true)
    {
        _shoe->shuffle();
        _player->onShuffle();

        while (!_shoe->needsShuffle())
        {
            if (Config::instance()->isDebugMode)
            {
                std::cout << "Game:" << gameCounter << " start" << std::endl;
            }

            auto isDone = step();
            if (!isDone)
            {
                gameCounter--;
            }

            if (Config::instance()->isDebugMode)
            {
                std::cout << std::endl;
            }

            if (++gameCounter == Config::instance()->game)
                goto end;
        }
    }
end:
    showResult(gameCounter);
}

void Simulation::showResult(int gameCount)
{
    std::cout << "Simulation results" << std::endl;
    if (Config::instance()->displaysDetailResult)
    {
        std::cout << _dealer->toString() << std::endl;
        std::cout << _player->toString() << std::endl;
    }
    std::cout << "House Edge:" << (float)_player->houseEdge(gameCount) << "%" << std::endl;
}
