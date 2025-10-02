#include <nlohmann/json.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include "chessboard.h"
#include <vector>
#include <map>

typedef websocketpp::server<websocketpp::config::asio> server;
using json = nlohmann::json;

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg){
    std::string payload = msg->get_payload();
    json j = json::parse(payload);
    std::string type = j["type"];
    

    if (type == "ChessboardState") {
        Chessboard chessboard(8, 8);
        chessboard.setPieceAt(0,  0, { {0, 0}, "rook", "Chess_rlt45.svg", { {1, 0, true, false}, {0, 1, true, false}, {-1, 0, true, false}, {0, -1, true, false} }, {}, false });
        
        json response;
        chessboard.to_json(response);
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }
    else if (type == "LoadGame") {
        
    }

    s->send(hdl, "{big working:'wow'}", msg->get_opcode());
}
// void on_open(websocketpp::connection_hdl hdl){

// }
// void on_close(websocketpp::connection_hdl hdl){

// }

int main() {
    server chessServer;
    try {
        chessServer.set_access_channels(websocketpp::log::alevel::all);
        chessServer.clear_access_channels(websocketpp::log::alevel::frame_payload);

        chessServer.set_pong_timeout(10000);

        chessServer.set_pong_handler([](websocketpp::connection_hdl, std::string) {
            return true;
        });

        chessServer.init_asio();
        chessServer.set_http_handler([&chessServer](websocketpp::connection_hdl hdl) {
            auto con = chessServer.get_con_from_hdl(hdl);
            con->append_header("Access-Control-Allow-Origin", "*");
        });

        chessServer.set_message_handler(bind(&on_message, &chessServer, std::placeholders::_1, std::placeholders::_2));
        // chessServer.set_open_handler(bind(&on_open, std::placeholders::_1));
        // chessServer.set_close_handler(bind(&on_close, std::placeholders::_1));
        chessServer.set_reuse_addr(true);
        chessServer.listen(9002);
        chessServer.start_accept();

        chessServer.run();
    } catch (websocketpp::exception const & e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

}