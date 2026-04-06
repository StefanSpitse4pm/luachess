//
// Created by stefanspitse on 3/28/26.
//

#include "Handler.h"

Handler::ActionFn Handler::route(std::unordered_map<std::string, ActionFn> actionMap, std::string action)
{
    if (const auto it = actionMap.find(action); it != actionMap.end())
    {
        return it->second;
    }
    throw std::invalid_argument("Unknown action: " + action);
}
