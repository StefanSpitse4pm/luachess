//
// Created by stefanspitse on 5/19/26.
//

#ifndef LUACHESS_WEBSOCKET_H
#define LUACHESS_WEBSOCKET_H

#endif // LUACHESS_WEBSOCKET_H

#include "Handlers/Games/GameHandler.h"
#include "Handlers/Handler.h"
#include "Handlers/Rooms/RoomHandler.h"
#include <memory>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

class Websocket
{
    public:
    Websocket() : roomHandler{}, gameHandler{roomHandler}, handlerMap{[&]()
    {
        std::unordered_map<std::string, std::unique_ptr<Handler>> m;
        m.emplace("Room", std::make_unique<RoomHandler>());
        m.emplace("Game", std::make_unique<GameHandler>(roomHandler));
        return m;
    }()}{};
    virtual ~Websocket() = default;
        virtual void onMessage(server* s, const websocketpp::connection_hdl& hdl, const server::message_ptr& msg) = 0;
        std::unique_ptr<Handler> findHandler(std::string type);
    private:
        std::unordered_map<std::string, std::unique_ptr<Handler>> handlerMap;
        RoomHandler roomHandler;
        GameHandler gameHandler;
};
