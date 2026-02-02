//
// Created by stefanspitse on 2/2/26.
//

#ifndef LUACHESS_ACTIONCONTEXT_H
#define LUACHESS_ACTIONCONTEXT_H
#include <string>
#include <websocketpp/common/connection_hdl.hpp>

struct UserConext {
    std::string username;
    websocketpp::connection_hdl hdl;
};

struct RoomContext {
    std::string roomName;
};

struct ActionContext {
    std::string action;

    UserConext userContext;
    RoomContext roomContext;
};

#endif //LUACHESS_ACTIONCONTEXT_H