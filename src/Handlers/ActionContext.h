/*
* MIT License

* Copyright (c) [year] [fullname]

* Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
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
    u_int32_t gameId = 0;
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

struct Notification
{
    std::vector<SessionContext> recipients;
    std::string message;
};

struct ActionContext
{
    std::string action;
    server* serverPtr;

    SessionContext sessionContext;
    RoomContext roomContext;
    GameContext gameContext;

    mutable std::vector<Notification> pendingNotifications;
};

#endif // LUACHESS_ACTIONCONTEXT_H
