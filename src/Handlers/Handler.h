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
    static void sendError(const ActionContext& ctx, const std::string& message);
    void notify(SessionContext);

};

#endif // LUACHESS_ACTION_H
