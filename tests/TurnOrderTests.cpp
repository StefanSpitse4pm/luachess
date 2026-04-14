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
    auto player1 = std::make_unique<Player>("Player1");
    auto player2 = std::make_unique<Player>("Player2");
    Player& player1Ref = *player1;
    Player& player2Ref = *player2;

    players.push_back(std::move(player1));
    players.push_back(std::move(player2));
    auto turnOrder = TurnOrder(players, player1Ref);

    turnOrder.turnTo(player2Ref);
    ASSERT_EQ(player2Ref.get_id(), turnOrder.getCurrentPlayer().get_id());
}

TEST_F(TurnOrderTests, TurnTo_PlayerAlreadyActivePlayer_ShouldNotTurn)
{

    auto player1 = std::make_unique<Player>("Player1");
    auto player2 = std::make_unique<Player>("Player2");
    Player& player1Ref = *player1;

    players.push_back(std::move(player1));
    auto turnOrder = TurnOrder(players, player1Ref);

    turnOrder.turnTo(player1Ref);
    ASSERT_EQ(player1Ref.get_id(), turnOrder.getCurrentPlayer().get_id());
}

TEST_F(TurnOrderTests, TurnTo_PlayerNotInsidePlayers_ShouldThrowInvalidArgument)
{
    auto player1 = std::make_unique<Player>("Player1");
    auto player2 = std::make_unique<Player>("Player2");
    Player& player1Ref = *player1;
    Player& player2Ref = *player2;

    players.push_back(std::move(player1));
    auto turnOrder = TurnOrder(players, player1Ref);

    ASSERT_THROW(turnOrder.turnTo(player2Ref), std::invalid_argument);
}

TEST_F(TurnOrderTests, DefaultTurnOrder_TurnToNextPlayer_ShouldNotWrap)
{

    auto player1 = std::make_unique<Player>("Player1");
    auto player2 = std::make_unique<Player>("Player2");
    Player& player1Ref = *player1;
    Player& player2Ref = *player2;

    players.push_back(std::move(player1));
    players.push_back(std::move(player2));
    auto turnOrder = TurnOrder(players, player1Ref);

    ASSERT_NO_THROW(turnOrder.defaultTurnOrder());
    ASSERT_EQ(player2Ref.get_id(), turnOrder.getCurrentPlayer().get_id());
}

TEST_F(TurnOrderTests, DefaultTurnOrder_TurnToNextPlayer_ShouldWrap)
{

    auto player1 = std::make_unique<Player>("Player1");
    auto player2 = std::make_unique<Player>("Player2");
    Player& player1Ref = *player1;
    Player& player2Ref = *player2;

    players.push_back(std::move(player1));
    players.push_back(std::move(player2));
    auto turnOrder = TurnOrder(players, player2Ref);

    ASSERT_NO_THROW(turnOrder.defaultTurnOrder());
    ASSERT_EQ(player1Ref.get_id(), turnOrder.getCurrentPlayer().get_id());
}
