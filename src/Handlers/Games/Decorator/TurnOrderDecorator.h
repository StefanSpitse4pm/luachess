//
// Created by stefanspitse on 4/12/26.
//
#include "GameDecorator.h"

#ifndef LUACHESS_TURNORDERDECORATOR_H
#define LUACHESS_TURNORDERDECORATOR_H
#include "GameDecorator.h"

class TurnOrderDecorator: GameDecorator {
  public:
    explicit TurnOrderDecorator(Game& source) : GameDecorator(source){};
    void start() const override;
    [[nodiscard]] nlohmann::json applyMove(const sendMove& move) const override;
};



#endif //LUACHESS_TURNORDERDECORATOR_H
