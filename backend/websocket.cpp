#include <nlohmann/json.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include "piece.h"
#include <vector>

typedef websocketpp::server<websocketpp::config::asio> server;
using json = nlohmann::json;

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg){
    std::string payload = msg->get_payload();
    json j = json::parse(payload);
    std::string type = j["type"];
    
    if (type == "ChessboardState") {


        std::vector<Move> possibleMoves;
        possibleMoves.push_back({0, 1, false, true});
        Piece p1(1, 1, "pawn", false, "/Chess_plt45.svg");
        Piece p2(1, 2, "pawn", false, "/Chess_plt45.svg");
        p1.setPossibleMoves(possibleMoves);
        p2.setPossibleMoves(possibleMoves);
        std::vector<Piece> pieces = {p1, p2};
        json response = json::array();

        for (auto& piece : pieces) 
        {
            json pieceJson;
            piece.to_json(pieceJson);
            response.push_back(pieceJson);
        }


        s->send(hdl, response.dump(), websocketpp::frame::opcode::text);
        return;
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