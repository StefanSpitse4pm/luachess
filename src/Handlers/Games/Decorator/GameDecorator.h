//
// Created by stefanspitse on 4/10/26.
//

#ifndef LUACHESS_GAMEDECORATOR_H
#define LUACHESS_GAMEDECORATOR_H
#include "../Game.h"
#include "../../ActionContext.h"
#include <nlohmann/json_fwd.hpp>

class GameDecorator : public Game
{
  public:
    explicit GameDecorator(Game& source) : wrapped(source)
    {

    }
    void start() const override;
    void stop() override;
    [[nodiscard]] nlohmann::json applyMove(const sendMove& move) const override;
  protected:
    Game& wrapped;

};

#endif //LUACHESS_GAMEDECORATOR_H


