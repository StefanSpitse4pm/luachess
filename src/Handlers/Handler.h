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

    using ActionFn = std::function<nlohmann::json(const ActionContext&)>;
    virtual nlohmann::json action(std::string action, const ActionContext& ctx) = 0;
    ActionFn route(std::unordered_map<std::string, ActionFn> actionMap, std::string action);
};

#endif // LUACHESS_ACTION_H
