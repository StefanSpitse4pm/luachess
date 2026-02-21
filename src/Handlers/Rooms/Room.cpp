//
// Created by stefanspitse on 1/31/26.
//

#include "Room.h"

#include <nlohmann/json.hpp>

constexpr int MAX_PLAYER_COUNT = 2;

void Room::addUser(const SessionContext& sessionContext)
{
    if (sessionContext.player->get_username().empty())
    {
        throw std::invalid_argument("username is empty");
    }

    if (sessionContexts.size() + 1 > MAX_PLAYER_COUNT)
    {
        throw std::out_of_range("get_player_hdl() > MAX_PLAYER_COUNT");
    }

    sessionContexts.emplace_back(sessionContext);
}

void Room::removeUser(SessionContext userContext)
{
    auto it = std::find_if(sessionContexts.begin(), sessionContexts.end(),
                           [&userContext](const SessionContext& ctx) { return ctx.player->get_username() == userContext.player->get_username(); });
    if (it != sessionContexts.end())
    {
        sessionContexts.erase(it);
    }
}

nlohmann::json Room::toJson() const
{
    std::vector<std::string> usernames;
    usernames.reserve(sessionContexts.size());
    for (const auto& [username, hdl] : sessionContexts)    {
        usernames.push_back(username->get_username());
    }
    return {
        {"roomName", roomName},
        {"roomSize", MAX_PLAYER_COUNT},
        {"filledSpots", sessionContexts.size()},
        {"players", usernames},
    };
}

const std::vector<SessionContext>& Room::getSessionContexts() const
{
    return sessionContexts;
}
