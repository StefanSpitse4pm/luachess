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
