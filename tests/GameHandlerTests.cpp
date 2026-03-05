//
// Created by stefanspitse on 3/5/26.
//

#include "gtest/gtest.h"
#include "../src/Handlers/Games/GameHandler.h"

TEST(GameHandlerStartGame, StartGame_GameTypeIsEmpty_ThrowsInvalidArgument)
{
    RoomHandler roomHandler;
    GameHandler gameHandler(roomHandler);
    ActionContext ctx;
    ctx.gameContext.gameType = "";
    ctx.roomContext.desiredRoomName = "TestRoom";

    EXPECT_THROW(gameHandler.startGame(ctx), std::invalid_argument);
}
