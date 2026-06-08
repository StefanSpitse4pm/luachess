//
// Created by stefanspitse on 5/19/26.
//

#include "Websocket.h"

std::unique_ptr<Handler> Websocket::findHandler(std::string typeOfHandler)
{
    if (const auto it = this->handlerMap.find(typeOfHandler); it != this->handlerMap.end())
    {
        return std::move(it->second);
    }
    throw std::invalid_argument("Unknown type of handler: " + typeOfHandler);
}
