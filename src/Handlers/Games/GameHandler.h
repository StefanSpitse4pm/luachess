//
// Created by stefanspitse on 2/14/26.
//

#ifndef LUACHESS_GAMEHANDLER_H
#define LUACHESS_GAMEHANDLER_H

#include "../Handler.h"
#include "../ActionContext.h"
#include "../../luaController.h"

class GameHandler : public Handler
{
    public:
        GameHandler() = default;
        ~GameHandler() override = default;

        void router(std::string action, const ActionContext& ctx) override;
        void startGame(ActionContext ctx);
    private:
        std::map<websocketpp::connection_hdl, luaRoomState, std::owner_less<websocketpp::connection_hdl>> games;
};

#endif // LUACHESS_GAMEHANDLER_H
