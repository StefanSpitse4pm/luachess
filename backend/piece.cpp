#include "piece.h"

Piece::Piece(int x, int y, std::string identifier, bool canJumpOverPieces, std::string image, std::vector<Move> possibleMoves, std::vector<Move> possibleTakes)
    : x(x), y(y), identifier(identifier), canJumpOverPieces(canJumpOverPieces), image(image), possibleMoves(possibleMoves), possibleTakes(possibleTakes) {}


void Piece::to_json(json& j) {
j = json{
    {"position", {{"row", x}, {"col", y}}},
    {"type", identifier},
    {"canJumpOverPieces", canJumpOverPieces},
    {"image", image},
    {"possibleMoves", json::array()},
    {"possibleTakes", json::array()}
    };

    for (const auto& move : possibleMoves) {
        j["possibleMoves"].push_back({
            {"dx", move.dx},
            {"dy", move.dy},
            {"repeat", move.repeat},
            {"basedOnLastMove", move.basedOnLastMove}
        });
    }

    for (const auto& take : possibleTakes) {
        j["possibleTakes"].push_back({
            {"dx", take.dx},
            {"dy", take.dy},
            {"repeat", take.repeat},
            {"basedOnLastMove", take.basedOnLastMove}
        });
    }
}
