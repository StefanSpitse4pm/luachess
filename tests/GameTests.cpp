//
// Created by stefanspitse on 4/7/26.
//
#include "../src/Handlers/Games/Engine/LuaEngine.h"
#include "../src/Handlers/Games/Game.h"
#include "../src/Handlers/Games/PlayerCreatedLuaGameFactory.h"

#include "gtest/gtest.h"

class GameTests: public ::testing::Test
{
    protected:
    std::unique_ptr<Engine> engine;
        std::unique_ptr<Chessboard> chessboard;
        ActionContext ctx;
        std::unique_ptr<Game> game;

        GameTests()
        :chessboard(std::make_unique<Chessboard>(8,8)), ctx()
        {
            engine = std::make_unique<LuaEngine>();
            game = std::make_unique<Game>(std::move(engine), std::move(chessboard), std::filesystem::current_path() / "lua" / "regularChess.lua");
        }
};

TEST_F(GameTests, applyMove_MovesPieceToNewPosition_UpdatesChessboard)
{
    Piece piece({0, 1}, "Knight", "knight.png", "White");
    game->getChessboard().setPieceAt(piece);
    sendMove move = {0, 1, 2, 2};
    json out = game->applyMove(move);
    assert(!game->getChessboard().isOccupied(0, 1));
    assert(game->getChessboard().isOccupied(2, 2));
}
