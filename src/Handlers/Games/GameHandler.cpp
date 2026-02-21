//
// Created by stefanspitse on 2/14/26.
//

#include "GameHandler.h"

void GameHandler::router(std::string action, const ActionContext& ctx)
{
	static const std::unordered_map<std::string, ActionFn> actionMap = {
	{"startGame", [this](const ActionContext& a_ctx) { startGame(a_ctx); }},
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
				ctx.SessionContext.hdl,
				R"({"type": "Error", "payload": {"message": "Cant find that action"}})",
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
    if (ctx.GameContext.gameType.empty())
    {
        ctx.serverPtr->send(ctx.SessionContext.hdl, R"({"type": "Error", "payload": {"message": "Missing game type"}})", websocketpp::frame::opcode::text);
        return;
    }

    if (ctx.GameContext.gameType == "PlayerCreatedLuaGame")
    {
        auto game = factories[0]->createGame(ctx);
        game->start();
        games.push_back(std::move(game));

    }
}
