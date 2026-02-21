//
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_PLAYERCREATEDLUAGAMEFACTORY_H
#define LUACHESS_PLAYERCREATEDLUAGAMEFACTORY_H
#include "GameFactory.h"

class PlayerCreatedLuaGameFactory : public GameFactory
{
  public:
    std::unique_ptr<Game> createGame(ActionContext ctx) override;
};

#endif // LUACHESS_PLAYERCREATEDLUAGAMEFACTORY_H
