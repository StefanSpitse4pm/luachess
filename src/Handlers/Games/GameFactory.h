//
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_GAMEFACTORY_H
#define LUACHESS_GAMEFACTORY_H
#include "../ActionContext.h"
#include "Game.h"

#include <memory>

class GameFactory
{
    public:
        virtual ~GameFactory() = default;
        virtual std::unique_ptr<Game> createGame(ActionContext ctx) = 0;
};

#endif // LUACHESS_GAMEFACTORY_H
