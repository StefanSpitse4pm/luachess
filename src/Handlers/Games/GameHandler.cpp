//
// Created by stefanspitse on 2/14/26.
//

#include "GameHandler.h"

json GameHandler::router(std::string action, const ActionContext& ctx)
{
    static const std::unordered_map<std::string, ActionFn> actionMap = {
        {"startGame", [this](const ActionContext& a_ctx) -> nlohmann::json { return startGame(a_ctx); }},
        {"boardState", [this](const ActionContext& a_ctx) -> nlohmann::json { return getBoardState(a_ctx); }},
    };
    auto it = actionMap.find(action);
    if (it != actionMap.end())
    {
        return it->second(ctx);
    }
    throw std::invalid_argument("Unknown action: " + action);
}

json GameHandler::startGame(const ActionContext& ctx)
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
        games.push_back(std::move(game));
        return response;
    }
    throw std::invalid_argument("Unsupported game type: " + ctx.gameContext.gameType);
}

json GameHandler::getBoardState(ActionContext ctx)
{
    if (ctx.gameContext.gameId == 0)
    {
        throw std::invalid_argument("Missing game ID");
    }

    auto it = std::ranges::find_if(
        games, [&ctx](const std::unique_ptr<Game>& game) { return game->getId() == ctx.gameContext.gameId; }
    );

    if (it == games.end())
    {
        throw std::invalid_argument("Game not found");
    }

    const json response = (*it)->getChessboard().to_json();
    return response;
}
