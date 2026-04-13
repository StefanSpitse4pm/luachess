//
// Created by stefanspitse on 4/12/26.
//

#include "../src/Handlers/ActionContext.h"
#include "../src/Handlers/Games/LuaGame.h"
#include "../src/Handlers/Games/PlayerCreatedLuaGameFactory.h"
#include "../src/Handlers/Games/TurnOrder.h"
#include "../src/Handlers/Rooms/Player.h"
#include "gtest/gtest.h"
#include <utility>
#include <vector>

class TurnOrderTests: public ::testing::Test
{
public:
    std::vector<std::unique_ptr<Player>> players;
  TurnOrderTests() = default;
};

TEST_F(TurnOrderTests, TurnOrder_CreateTurnOrder_ShouldNotFail)
{
    std::unique_ptr<Player> player1;
    players.push_back(std::move(player1));
    ASSERT_NO_THROW(auto turnOrder = TurnOrder(players, players[0]));
}
