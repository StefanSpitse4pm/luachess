//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ACTION_H
#define LUACHESS_ACTION_H
#include "ActionContext.h"
#include <nlohmann/json.hpp>
#include <ranges>
#include <string>
#include <type_traits>
#include <websocketpp/roles/server_endpoint.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;


class Handler
{
  public:
	virtual ~Handler() = default;

	using ActionFn = std::function<void(const ActionContext&)>;
	virtual nlohmann::json router(std::string action, const ActionContext& ctx) = 0;
    static void sendError(const ActionContext& ctx, const std::string& message);

    template <std::ranges::range R>
    requires std::same_as<std::ranges::range_value_t<R>, SessionContext>
    void notify(R sessions, std::string& message, server* serverPtr)
    {
        for (const auto& session : sessions)
        {
            serverPtr->send(session.hdl, message, websocketpp::frame::opcode::text);
        }
    }
};

#endif // LUACHESS_ACTION_H
