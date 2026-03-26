//
// Created by stefanspitse on 2/14/26.
//

#ifndef LUACHESS_GAMEHANDLER_H
#define LUACHESS_GAMEHANDLER_H

#include "../../luaController.h"
#include "../ActionContext.h"
#include "../Handler.h"
#include "../Rooms/Player.h"
#include "../Rooms/RoomHandler.h"
#include "Game.h"
#include "GameFactory.h"
#include "PlayerCreatedLuaGameFactory.h"

class GameHandler : public Handler
{
    public:
	GameHandler(RoomHandler& roomHandler) : roomHandler(roomHandler)
	{
          auto factory = std::make_unique<PlayerCreatedLuaGameFactory>();
          factories.push_back(std::move(factory));
	};
    json router(std::string action, const ActionContext& ctx) override;
    json startGame(const ActionContext& ctx);
    json getBoardState(ActionContext ctx);
    Game& getGameByGameId(const ActionContext& ctx);
    json onMove(const ActionContext& ctx);

  private:
	std::vector<std::unique_ptr<Game>> games;
	std::vector<std::unique_ptr<GameFactory>> factories;
    RoomHandler& roomHandler;
};

#endif // LUACHESS_GAMEHANDLER_H
