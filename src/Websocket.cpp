//
// Created by stefanspitse on 5/19/26.
//

#include "Websocket.h"

std::unique_ptr<Handler> Websocket::findHandler(std::string type)
{
    if (const auto it = this->handlerMap.find(type); it != this->handlerMap.end())
    {
        return std::move(it->second);
    }
}
