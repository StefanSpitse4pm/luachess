//
// Created by stefanspitse on 4/15/26.
//
#include "../src/Handlers/Games/Decorator/TurnOrderDecorator.h"

#include "../src/Handlers/Games/Engine/LuaEngine.h"
#include "../src/Chess/chessboard.h"
#include "../src/Handlers/Games/LuaGame.h"
#include "gtest/gtest.h"

class TurnOrderDecoratorTests: public ::testing::Test
{
    protected:
        std::vector<SessionContext> sessions;
        SessionContext s1;
        SessionContext s2;
        std::unique_ptr<Player> player1 = std::make_unique<Player>("Player1");
        std::unique_ptr<Player> player2 = std::make_unique<Player>("Player2");
        std::unique_ptr<LuaEngine> engine = std::make_unique<LuaEngine>();
        std::unique_ptr<Chessboard> board = std::make_unique<Chessboard>(8, 8);
        std::unique_ptr<LuaGame> newGame = std::make_unique<LuaGame>(
            std::move(engine), std::move(board), std::filesystem::current_path() / "lua" / "regularChess.lua"
        );
};

TEST_F(TurnOrderDecoratorTests, createTurnOrderFromSessionContexts_ShouldReturnPlayers)
{
    s1.player = player1.get();
    s2.player = player2.get();
    sessions.push_back(s1);
    sessions.push_back(s2);
    newGame->addPlayers(sessions);
    const auto turnOrderNewGame = std::make_unique<TurnOrderDecorator>(*newGame);

    ASSERT_NO_THROW(turnOrderNewGame->createTurnOrderFromSessionContexts());

    ASSERT_EQ(turnOrderNewGame->getTurnOrder().getCurrentPlayer().get_username(), "Player1");

    turnOrderNewGame->getTurnOrder().defaultTurnOrder();
    ASSERT_EQ(turnOrderNewGame->getTurnOrder().getCurrentPlayer().get_username(), "Player2");

}
