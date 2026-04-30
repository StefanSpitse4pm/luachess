//
// Created by stefanspitse on 4/7/26.
//
#include "../src/Handlers/Games/Engine/LuaEngine.h"
#include "../src/Handlers/Games/LuaGame.h"
#include "../src/Handlers/Games/PlayerCreatedLuaGameFactory.h"

#include "gtest/gtest.h"

class GameTests: public ::testing::Test
{
    protected:
    std::unique_ptr<Engine> engine;
        std::unique_ptr<Chessboard> chessboard;
        ActionContext ctx;
        std::unique_ptr<LuaGame> game;

        GameTests()
        :chessboard(std::make_unique<Chessboard>(8,8)), ctx()
        {
            engine = std::make_unique<LuaEngine>();
            game = std::make_unique<LuaGame>(std::move(engine), std::move(chessboard), "/home/stefanspitse/CLionProjects/luachess/src/lua/regularChess.lua"); // temporary
            game->start();
        }
};

TEST_F(GameTests, applyMove_MovesPieceToNewPosition_UpdatesChessboard)
{
    sendMove move = {0, 1, 0, 2};
    json out = this->game->applyMove(move);
    assert(!this->game->getChessboard().isOccupied(0, 1));
    assert(this->game->getChessboard().isOccupied(0, 2));
}
