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
// Created by stefanspitse on 2/14/26.
//

#include "GameHandler.h"

nlohmann::json GameHandler::action(std::string action, const ActionContext& ctx)
{
    static const std::unordered_map<std::string, ActionFn> actionMap = {
        {"startGame", [this](const ActionContext& a_ctx) -> nlohmann::json { return startGame(a_ctx); }},
        {"boardState", [this](const ActionContext& a_ctx) -> nlohmann::json { return getBoardState(a_ctx); }},
        {"move", [this](const ActionContext& a_ctx) -> nlohmann::json { return onMove(a_ctx); }},
    };
    return route(actionMap, action)(ctx);
}

Game& GameHandler::getGameByGameId(const ActionContext& ctx)
{
    if (ctx.gameContext.gameId == 0)
    {
        throw std::invalid_argument("Missing game ID");
    }

    const auto it = std::ranges::find_if(
        games, [&ctx](const std::unique_ptr<Game>& game) { return game->getId() == ctx.gameContext.gameId; }
    );

    if (it == games.end())
    {
        throw std::invalid_argument("Game not found");
    }

    return **it;
}

nlohmann::json GameHandler::startGame(const ActionContext& ctx)
{
    if (ctx.gameContext.gameType.empty())
    {
        throw std::invalid_argument("Missing game type");
    }

    if (ctx.roomContext.desiredRoomName.empty())
    {
        throw std::invalid_argument("Missing desired room name");
    }

    if (ctx.gameContext.gameType == "PlayerCreatedLuaGame")
    {
        auto game = factories[0]->createGame(ctx);

        const Room room = roomHandler.findRoomByName(ctx.roomContext.desiredRoomName);
        if (!room.isReady())
        {
            throw std::invalid_argument("Room is not ready");
        }

        game->addPlayers(room.getSessionContexts());

        roomHandler.removeRoom(room);

        game->start();
        const json response = game->toJson();
        ctx.pendingNotifications.push_back({game->getSessionContexts(), response.dump()});
        games.push_back(std::move(game));
        return response;
    }
    throw std::invalid_argument("Unsupported game type: " + ctx.gameContext.gameType);
}

nlohmann::json GameHandler::getBoardState(ActionContext ctx)
{
    Game& game = getGameByGameId(ctx);
    return game.getChessboard().toJson();
}

nlohmann::json GameHandler::onMove(const ActionContext& ctx)
{
    Game& game = getGameByGameId(ctx);

    if (ctx.gameContext.send == nullptr)
    {
        throw std::invalid_argument("Missing move payload");
    }


    ctx.pendingNotifications.push_back({game.getSessionContexts(), board.dump()});
    return board;
}
