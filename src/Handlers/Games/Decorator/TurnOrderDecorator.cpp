/*
MIT License

Copyright (c) 2026 Stefan Spitse

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//
// Created by stefanspitse on 4/12/26.
//

#include "TurnOrderDecorator.h"

#include "../Engine/Engine.h"
#include "../Engine/LuaEngine.h"
#include <nlohmann/json.hpp>

#include <sol/function_types_templated.hpp>
#include <stdexcept>

void TurnOrderDecorator::start()
{
    if (turnOrder == nullptr)
    {
        createTurnOrderFromSessionContexts();
    }
    Engine& base = wrapped.getEngine();
    base.addTurnOrder(*turnOrder);
    GameDecorator::start();
}

nlohmann::json TurnOrderDecorator::applyMove(const sendMove& move) const
{
    if (turnOrder == nullptr)
    {
        const_cast<TurnOrderDecorator*>(this)->createTurnOrderFromSessionContexts();
    }

    if (!move.publicPlayerId.empty())
    {
        isPlayersTurn(move);
    }

    nlohmann::json result = GameDecorator::applyMove(move);
    return result;
}

void TurnOrderDecorator::createTurnOrderFromSessionContexts()
{
    std::vector<std::unique_ptr<Player>> players;
    Player* firstPlayer = nullptr;
    for (const auto& [player, hdl] : getSessionContexts())
    {
        if (player == nullptr)
        {
            throw std::invalid_argument("Player in session context is null");
        }

        auto created = std::make_unique<Player>(player->get_username());
        if (firstPlayer == nullptr)
        {
            firstPlayer = created.get();
        }
        players.push_back(std::move(created));
    }

    if (firstPlayer == nullptr)
    {
        throw std::invalid_argument("No players found in session contexts");
    }

    turnOrder = new TurnOrder(players, *firstPlayer);
}


void TurnOrderDecorator::isPlayersTurn(const sendMove& move) const
{
    if (turnOrder->getCurrentPlayer().get_public_id() != move.publicPlayerId)
    {
        throw std::invalid_argument("It's not this player's turn");
    }
}

nlohmann::json TurnOrderDecorator::toJson() const
{
    nlohmann::json game = GameDecorator::toJson();
    game.merge_patch(getTurnOrder().toJson());
    return game;
}
