//
// Created by stefanspitse on 4/7/26.
//
#ifndef LUACHESS_GAMEDECORATOR_H
#define LUACHESS_GAMEDECORATOR_H
#include "../ActionContext.h"
#include <nlohmann/json_fwd.hpp>

#endif // LUACHESS_GAMEDECORATOR_H

class Game
{
  public:
    virtual ~Game() = default;
    virtual void start() const = 0;
    virtual void stop() = 0;
    [[nodiscard]] virtual nlohmann::json applyMove(const sendMove& move) const = 0;
};
