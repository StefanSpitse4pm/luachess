//
// Created by stefanspitse on 6/8/26.
//
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#ifndef LUACHESS_ACTIONHANDLER_H
#define LUACHESS_ACTIONHANDLER_H

typedef websocketpp::server<websocketpp::config::asio> server;

class actionHandler
{
    virtual void onMessage(server* s, const websocketpp::connection_hdl& hdl, const server::message_ptr& msg)
};

#endif // LUACHESS_ACTIONHANDLER_H
