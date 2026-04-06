// MIT License
//
// Copyright (c) 2026 Stefan Spitse
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
// Created by stefanspitse on 1/31/26.
//

#include "Room.h"

#include <nlohmann/json.hpp>

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
    auto it = std::find_if(
        sessionContexts.begin(), sessionContexts.end(), [&userContext](const SessionContext& ctx)
        { return ctx.player->get_username() == userContext.player->get_username(); }
    );
    if (it != sessionContexts.end())
    {
        sessionContexts.erase(it);
    }
}

nlohmann::json Room::toJson() const
{
    std::vector<std::string> usernames;
    nlohmann::json playersArray = nlohmann::json::array();
    usernames.reserve(sessionContexts.size());
    for (const auto& [player, hdl] : sessionContexts)
    {
        usernames.push_back(player->get_username());
        playersArray.push_back({{"username", player->get_username()}, {"playerId", player->get_id()}});
    }
    return {
        {"roomName", roomName},
        {"roomSize", MAX_PLAYER_COUNT},
        {"filledSpots", sessionContexts.size()},
        {"players", usernames},
        {"playersInfo", playersArray}
    };
}

bool Room::isReady() const
{
    return sessionContexts.size() == MAX_PLAYER_COUNT;
}
