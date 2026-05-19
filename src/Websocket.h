//
// Created by stefanspitse on 5/19/26.
//

#ifndef LUACHESS_WEBSOCKET_H
#define LUACHESS_WEBSOCKET_H

#endif // LUACHESS_WEBSOCKET_H

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include "Handlers/Handler.h"
#include "Handlers/Games/GameHandler.h"
#include "Handlers/Rooms/RoomHandler.h"
#include <memory>

typedef websocketpp::server<websocketpp::config::asio> server;
class Websocket
{

    public:
        Websocket()
        {
            RoomHandler roomHandler;
            GameHandler gameHandler(roomHandler);
        };

        virtual ~Websocket() = default;
        virtual void onMessage(server* s, const websocketpp::connection_hdl& hdl, const server::message_ptr& msg) = 0;
        std::unique_ptr<Handler> findHandler(std::string type);

};
