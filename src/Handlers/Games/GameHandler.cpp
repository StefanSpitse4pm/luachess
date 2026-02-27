//
// Created by stefanspitse on 2/14/26.
//

#include "GameHandler.h"

void GameHandler::router(std::string action, const ActionContext& ctx)
{
    static const std::unordered_map<std::string, ActionFn> actionMap = {
        {"startGame", [this](const ActionContext& a_ctx) { startGame(a_ctx); }},
        {"onMove", [this](const ActionContext& a_ctx) { onMove(a_ctx); }},
    };
    auto it = actionMap.find(action);
    if (it != actionMap.end())
    {
        try
        {
            it->second(ctx);
        }
        catch (...)
        {
            ctx.serverPtr->send(
                ctx.sessionContext.hdl, R"({"type": "Error", "payload": {"message": "Cant find that action"}})",
                websocketpp::frame::opcode::text
            );
        }
    }
    else
    {
        throw std::invalid_argument("Unknown action: " + action);
    }
}

void GameHandler::startGame(ActionContext ctx)
{
    if (ctx.gameContext.gameType.empty())
    {
        ctx.serverPtr->send(
            ctx.sessionContext.hdl, R"({"type": "Error", "payload": {"message": "Missing game type"}})",
            websocketpp::frame::opcode::text
        );
        return;
    }

    if (ctx.roomContext.desiredRoomName.empty())
    {
        ctx.serverPtr->send(
            ctx.sessionContext.hdl, R"({"type": "Error", "payload": {"message": "Missing room name"}})",
            websocketpp::frame::opcode::text
        );
        return;
    }

    if (ctx.gameContext.gameType == "PlayerCreatedLuaGame")
    {
        auto game = factories[0]->createGame(ctx);

        const Room room = roomHandler.findRoomByName(ctx.roomContext.desiredRoomName);
        if (!room.isReady())
        {
            ctx.serverPtr->send(
                ctx.sessionContext.hdl, R"({"type": "Error", "payload": {"message": "Room is not ready"}})",
                websocketpp::frame::opcode::text
            );
            return;
        }

        game->addPlayers(room.getSessionContexts());
        roomHandler.removeRoom(room);

        game->start();
        const json response = game->getChessboard().to_json();
        games.push_back(std::move(game));
        ctx.serverPtr->send(ctx.sessionContext.hdl, response.dump(), websocketpp::frame::opcode::text);
    }
}

void GameHandler::onMove(ActionContext ctx)
{
}
