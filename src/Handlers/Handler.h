//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ACTION_H
#define LUACHESS_ACTION_H
#include "ActionContext.h"
#include <string>

class Handler
{
  public:
    virtual ~Handler() = default;

    using ActionFn = std::function<void(const ActionContext&)>;
    virtual void router(std::string action, const ActionContext& ctx) = 0;
};

#endif // LUACHESS_ACTION_H
