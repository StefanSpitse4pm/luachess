//
// Created by stefanspitse on 5/19/26.
//

#ifndef LUACHESS_LUACHESS_H
#define LUACHESS_LUACHESS_H
#include "Websocket.h"

#endif // LUACHESS_LUACHESS_H



class Luachess : Websocket
{
    public:
        void onMessage(server* s, const websocketpp::connection_hdl& hdl, const server::message_ptr& msg) override;

        template <std::ranges::range R> requires std::same_as<std::ranges::range_value_t<R>, SessionContext>
        void notify(const R& sessions, const std::string& message, server* serverPtr);
    };
