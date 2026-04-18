//
// Created by stefanspitse on 4/12/26.
//
#pragma once
#include "../TurnOrder.h"
#include "GameDecorator.h"
#include "../../Rooms/Player.h"

#ifndef LUACHESS_TURNORDERDECORATOR_H
#define LUACHESS_TURNORDERDECORATOR_H

class TurnOrderDecorator: public GameDecorator {
    public:
    explicit TurnOrderDecorator(Game& source) : GameDecorator(source)
    {
    }

    ~TurnOrderDecorator() override
    {
        delete turnOrder;
        turnOrder = nullptr;
    }

    void createTurnOrderFromSessionContexts();
    void isPlayersTurn(const sendMove& move) const;
    void start() override;
    [[nodiscard]] nlohmann::json applyMove(const sendMove& move) const override;
    [[nodiscard]] nlohmann::json toJson() const override;
    [[nodiscard]] TurnOrder& getTurnOrder() const
    {
        return *turnOrder;
    }
    protected:
        TurnOrder *turnOrder = nullptr;
};



#endif //LUACHESS_TURNORDERDECORATOR_H
