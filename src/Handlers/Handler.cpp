//
// Created by stefanspitse on 2/28/26.
//

#include "Handler.h"

void Handler::sendError(const ActionContext& ctx, const std::string& message)
{
    ctx.serverPtr->send(
        ctx.sessionContext.hdl, R"({"type": "Error", "payload": {"message": ")" + message + R"("}})",
        websocketpp::frame::opcode::text
    );
}

template <std::ranges::range R>
    requires std::same_as<std::ranges::range_value_t<R>, SessionContext>
void Handler::notify(R sessions, std::string& message, server* serverPtr)
{
    for (const auto& session : sessions)
    {
        serverPtr->send(session.hdl, message, websocketpp::frame::opcode::text);
    }
}
