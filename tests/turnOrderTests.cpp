//
// Created by stefanspitse on 4/12/26.
//

#include "../src/Handlers/ActionContext.h"
#include "../src/Handlers/Games/LuaGame.h"
#include "../src/Handlers/Games/PlayerCreatedLuaGameFactory.h"
#include "../src/Handlers/Games/turnOrder.h"
#include "../src/Handlers/Rooms/Player.h"
#include "gtest/gtest.h"
#include <vector>

class turnOrderTests: public ::testing::Test
{
public:
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    std::vector<std::unique_ptr<Player>>& players;

    std::unique_ptr<PlayerCreatedLuaGameFactory> factory = std::make_unique<PlayerCreatedLuaGameFactory>();
    ActionContext ctx;
    std::unique_ptr<LuaGame> game;

    turnOrderTests(
        const std::unique_ptr<Player>& player1, const std::unique_ptr<Player>& player2,
        std::vector<std::unique_ptr<Player>>& players, const std::unique_ptr<LuaGame>& game
    )
        : player1(player1), player2(player2), players(players), game(game)
    {
    }
};

TEST_F(turnOrderTests, TurnOrder_CreateTurnOrder_ShouldNotFail)
{
    players.push_back(player1);
    players.push_back(player2);
    turnOrder(players, player1);
}
