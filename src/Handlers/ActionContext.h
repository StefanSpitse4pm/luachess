//
// Created by stefanspitse on 2/2/26.
//

#ifndef LUACHESS_ACTIONCONTEXT_H
#define LUACHESS_ACTIONCONTEXT_H

class Room;

#include <string>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/roles/server_endpoint.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

struct UserContext
{
    std::string username;
    websocketpp::connection_hdl hdl;
};

struct RoomContext
{
    Room* room = nullptr; // pointer to the resolved Room (may be null for CreateRoom)
    std::string desiredRoomName; // incoming room name from payload (used to create or lookup rooms)
};

struct ActionContext
{
    std::string action;
    server* serverPtr;

    UserContext userContext;
    RoomContext roomContext;
};

#endif // LUACHESS_ACTIONCONTEXT_H
