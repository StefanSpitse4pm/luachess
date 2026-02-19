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

class GameHandler : public Handler
{
    public:
        GameHandler() = default;

        void router(std::string action, const ActionContext& ctx) override;
        void startGame(ActionContext ctx);
    private:
	    std::vector<std::unique_ptr<Game>> games;
		std::vector<GameFactory> factories;
};

#endif // LUACHESS_GAMEHANDLER_H
