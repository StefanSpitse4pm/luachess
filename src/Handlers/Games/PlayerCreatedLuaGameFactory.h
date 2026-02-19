//
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_PLAYERCREATEDLUAGAMEFACTORY_H
#define LUACHESS_PLAYERCREATEDLUAGAMEFACTORY_H
#include "GameFactory.h"

class PlayerCreatedLuaGameFactory : GameFactory
{
  public:
	std::unique_ptr<Game> createGame() override;
};

#endif // LUACHESS_PLAYERCREATEDLUAGAMEFACTORY_H
