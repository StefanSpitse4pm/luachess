//
// Created by stefanspitse on 3/5/26.
//

#include "../src/Handlers/Games/GameHandler.h"
#include "gtest/gtest.h"
#include <iostream>

class GameHandlerTest : public ::testing::Test
{
  protected:
    RoomHandler roomHandler;
    GameHandler gameHandler;
    ActionContext ctx;

    GameHandlerTest() : gameHandler(roomHandler), ctx()
    {
        ctx.gameContext.gameType = "PlayerCreatedLuaGame";
        ctx.roomContext.desiredRoomName = "TestRoom";
    }
};

TEST_F(GameHandlerTest, StartGame_GameTypeIsEmpty_ShouldThrowsInvalidArgument)
{
    ctx.gameContext.gameType = "";

    try
    {
        gameHandler.startGame(ctx);
        FAIL();
    }
    catch (const ::std::invalid_argument& e)
    {
        EXPECT_STREQ("Missing game type", e.what());
    }
    catch (...)
    {
        FAIL();
    }
}

TEST_F(GameHandlerTest, StartGame_DesiredRoomNameIsEmpty_ShouldThrowsInvalidArgument)
{
    ctx.roomContext.desiredRoomName = "";

    try
    {
        gameHandler.startGame(ctx);
        FAIL();
    }
    catch (const ::std::invalid_argument& e)
    {
        EXPECT_STREQ("Missing desired room name", e.what());
    }
    catch (...)
    {
        FAIL();
    }
    EXPECT_THROW(gameHandler.startGame(ctx), std::invalid_argument);
}

TEST_F(GameHandlerTest, StartGame_GametypeIsInvalid_ShouldThrowsInvalidArgument)
{
    ctx.gameContext.gameType = "Gibberish";
    try
    {
        gameHandler.startGame(ctx);
        FAIL();
    }
    catch (const std::invalid_argument& e)
    {
        std::string m = "Unsupported game type: " + ctx.gameContext.gameType;
        EXPECT_STREQ(m.c_str(), e.what());
    }
    catch (...)
    {
        FAIL();
    }
}

TEST_F(GameHandlerTest, StartGame_RoomNameDoesNotExist_ShouldThrowInvalidArgument)
{
    ctx.roomContext.desiredRoomName = "Gibberish";
    try
    {
        gameHandler.startGame(ctx);
        FAIL();
    }
    catch (const std::invalid_argument& e)
    {
        EXPECT_STREQ("Room not found", e.what());
    }
    catch (...)
    {
        FAIL();
    }
}

TEST_F(GameHandlerTest, StartGame_RoomIsNotReady_ShouldThrowInvalidArgument)
{
    const websocketpp::connection_hdl hdl;
    Player player("JohnDoe");
    SessionContext session;
    session.hdl = hdl;
    session.player = &player;
    ctx.sessionContext = session;

    roomHandler.createRoom(ctx);
    try
    {
        gameHandler.startGame(ctx);
        FAIL();
    }
    catch (const std::invalid_argument& e)
    {
        EXPECT_STREQ("Room is not ready", e.what());
    }
    catch (...)
    {
        FAIL(); // Did not get room is not ready error but did get an error.
    }
}
