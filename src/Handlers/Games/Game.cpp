//
// Created by stefanspitse on 2/19/26.
//

#include "Game.h"

void Game::start() const
{
    engine->setup(*chessboard);
    engine->initialize(filepath, *chessboard);
}

void Game::stop()
{
    // TODO: Implement game stop logic here
}
