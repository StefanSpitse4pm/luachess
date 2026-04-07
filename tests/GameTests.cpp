//
// Created by stefanspitse on 4/7/26.
//
#include "../src/Handlers/Games/Game.h"
#include "../src/Handlers/Games/PlayerCreatedLuaGameFactory.h"
#include "gtest/gtest.h"

class GameTests: public ::testing::Test
{
    protected:
        std::__detail::__unique_ptr_t<PlayerCreatedLuaGameFactory> engine;
        std::unique_ptr<Chessboard> chessboard;
        ActionContext ctx;
        std::unique_ptr<Game> game;

        GameTests()
        :chessboard(std::make_unique<Chessboard>(8,8)), ctx()
        {
            engine = std::make_unique<PlayerCreatedLuaGameFactory>();
            game = engine->createGame(ctx);
        }
};

TEST_F(GameTests, applyMove_MovesPieceToNewPosition_UpdatesChessboard)
{
    Piece piece({0, 1}, "Knight", "knight.png", "White");
    game->getChessboard().setPieceAt(piece);

    json out = game->applyMove(ctx);


}
