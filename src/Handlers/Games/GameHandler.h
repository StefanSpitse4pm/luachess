//
// Created by stefanspitse on 2/14/26.
//

#ifndef LUACHESS_GAMEHANDLER_H
#define LUACHESS_GAMEHANDLER_H

#include "../../luaController.h"
#include "../ActionContext.h"
#include "../Handler.h"
#include "Game.h"
#include "GameFactory.h"
#include "../Rooms/Player.h"
#include "PlayerCreatedLuaGameFactory.h"

class GameHandler : public Handler
{
    public:
	GameHandler() {
	    auto factory = std::make_unique<PlayerCreatedLuaGameFactory>();
	    factories.push_back(std::move(factory));
	};
	void router(std::string action, const ActionContext& ctx) override;
    void startGame(ActionContext ctx);
    void onMove(ActionContext ctx);

    private:
	std::vector<std::unique_ptr<Game>> games;
	std::vector<std::unique_ptr<GameFactory>> factories;
};

#endif // LUACHESS_GAMEHANDLER_H
