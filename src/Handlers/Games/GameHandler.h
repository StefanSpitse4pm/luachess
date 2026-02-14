//
// Created by stefanspitse on 2/14/26.
//

#ifndef LUACHESS_GAMEHANDLER_H
#define LUACHESS_GAMEHANDLER_H

#include "../Handler.h"
#include "../ActionContext.h"

class GameHandler : public Handler
{
    public:
        GameHandler() = default;
        ~GameHandler() override = default;

        void router(std::string action, const ActionContext& ctx);
        void startGame(ActionContext ctx);
};

#endif // LUACHESS_GAMEHANDLER_H
