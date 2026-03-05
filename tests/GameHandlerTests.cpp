//
// Created by stefanspitse on 3/5/26.
//

#include "gtest/gtest.h"
#include "../src/Handlers/Games/GameHandler.h"

class GameHandlerTest : public ::testing::Test
{
    protected:
        RoomHandler roomHandler;
        GameHandler gameHandler;
        ActionContext ctx;

        GameHandlerTest() : gameHandler(roomHandler), ctx()
        {
        }
};

TEST_F(GameHandlerTest, StartGame_GameTypeIsEmpty_ThrowsInvalidArgument)
{
    ctx.gameContext.gameType = "";
    ctx.roomContext.desiredRoomName = "TestRoom";

    EXPECT_THROW(gameHandler.startGame(ctx), std::invalid_argument);
}
