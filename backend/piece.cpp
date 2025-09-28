#include "piece.h"

Piece::Piece(int x, int y, std::string identifier, bool canJumpOverPieces, std::string image, std::vector<Move> possibleMoves, std::vector<Move> possibleTakes)
    : x(x), y(y), identifier(identifier), canJumpOverPieces(canJumpOverPieces), image(image), possibleMoves(possibleMoves), possibleTakes(possibleTakes) {}
Piece::~Piece() {}
