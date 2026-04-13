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
    ASSERT_NO_THROW(auto turnOrder = TurnOrder(players, *players[0]));
}

TEST_F(TurnOrderTests, TurnTo_SwitchActivePlayer_AssertEqualsPlayer2)
{

    std::unique_ptr<Player> player1 = std::make_unique<Player>("Player1");
    std::unique_ptr<Player> player2 = std::make_unique<Player>("Player2");
    Player& player1Ref = *player1;
    Player& player2Ref = *player2;

    players.push_back(std::move(player1));
    players.push_back(std::move(player2));
    auto turnOrder = TurnOrder(players, player1Ref);

    turnOrder.turnTo(player2Ref);
    ASSERT_EQ(player2Ref.get_id(), turnOrder.getCurrentPlayer().get_id());
}
