//
// Created by stefanspitse on 5/19/26.
//

#include "Websocket.h"

std::unique_ptr<Handler> Websocket::findHandler(std::string type)
{
    std::unordered_map<std::string, std::unique_ptr<Handler>> handlerMap;
    handlerMap.emplace("Room", std::make_unique<RoomHandler>(std::move(roomHandler)));
    handlerMap.emplace("Game", std::make_unique<GameHandler>(std::move(gameHandler)));

}
