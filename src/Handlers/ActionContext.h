//
// Created by stefanspitse on 2/2/26.
//

#ifndef LUACHESS_ACTIONCONTEXT_H
#define LUACHESS_ACTIONCONTEXT_H
#include "Rooms/Player.h"

class Room;

#include <string>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/roles/server_endpoint.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

struct sendMove
{
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
};

struct GameContext
{
    sendMove* send = nullptr;
    int boardSize = 8;
    std::string gameType;
};

struct SessionContext
{
    Player* player = nullptr;
    websocketpp::connection_hdl hdl;
};

struct RoomContext
{
    Room* room = nullptr;
    std::string desiredRoomName;
};

struct ActionContext
{
    std::string action;
    server* serverPtr;

    SessionContext sessionContext;
    RoomContext roomContext;
    GameContext gameContext;
};

#endif // LUACHESS_ACTIONCONTEXT_H
