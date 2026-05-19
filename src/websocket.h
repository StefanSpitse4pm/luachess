//
// Created by stefanspitse on 5/19/26.
//

#ifndef LUACHESS_WEBSOCKET_H
#define LUACHESS_WEBSOCKET_H

#endif // LUACHESS_WEBSOCKET_H

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
class websocket
{
    public:
        virtual ~websocket() = default;
        virtual void onMessage(server* s, const websocketpp::connection_hdl& hdl, const server::message_ptr& msg) = 0;

};
