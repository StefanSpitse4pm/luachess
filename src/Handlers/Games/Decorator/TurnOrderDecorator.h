//
// Created by stefanspitse on 4/12/26.
//
#pragma once
#include "../TurnOrder.h"
#include "GameDecorator.h"
#include "../../Rooms/Player.h"

#ifndef LUACHESS_TURNORDERDECORATOR_H
#define LUACHESS_TURNORDERDECORATOR_H

class TurnOrderDecorator: GameDecorator {
    public:
    explicit TurnOrderDecorator(Game& source) : GameDecorator(source)
    {
    }

    void createTurnOrderFromSessionContexts();
    void start() override;
        [[nodiscard]] nlohmann::json applyMove(const sendMove& move) const override;
    private:
        TurnOrder *turnOrder = nullptr;
};



#endif //LUACHESS_TURNORDERDECORATOR_H
