#include "chessboard.h"

void Chessboard::to_json(json& j) {
    j = json::array();
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            if (board[r][c].has_value()) {
                const Piece& piece = board[r][c].value();
                json pieceJson;
                pieceJson["position"] = {{"row", r}, {"col", c}};
                pieceJson["type"] = piece.type;
                pieceJson["canJumpOverPieces"] = piece.canJumpOverPieces;
                pieceJson["image"] = piece.image;

                pieceJson["possibleMoves"] = json::array();
                for (const auto& move : piece.possibleMoves) {
                    pieceJson["possibleMoves"].push_back({
                        {"dx", move.dx},
                        {"dy", move.dy},
                        {"repeat", move.repeat},
                        {"basedOnLastMove", move.basedOnLastMove}
                    });
                }

                pieceJson["possibleTakes"] = json::array();
                for (const auto& take : piece.possibleTakes) {
                    pieceJson["possibleTakes"].push_back({
                        {"dx", take.dx},
                        {"dy", take.dy},
                        {"repeat", take.repeat},
                        {"basedOnLastMove", take.basedOnLastMove}
                    });
                }
                
                j.push_back(pieceJson);
            }
        }
    }
}
