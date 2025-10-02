#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <optional>

struct Move {
    int dx;
    int dy;
    bool repeat = false;
    bool basedOnLastMove = false;
};

struct Piece {
    int position[2];   // position[0] = row, position[1] = col
    std::string type;
    std::string image;
    std::vector<Move> possibleMoves;
    std::vector<Move> possibleTakes;
    bool canJumpOverPieces = false;
};

using json = nlohmann::json;

class Chessboard {
public:
    Chessboard(int rows, int cols) : rows_(rows), cols_(cols), board(rows, std::vector<std::optional<Piece>>(cols, std::nullopt)) {}

    std::optional<Piece>& getPieceAt(int row, int col) {
        return board[row][col];
    }

    void setPieceAt(int row, int col, const Piece& piece) {
        board[row][col] = piece;
    }

    void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = Piece();
    }
    std::vector<std::vector<std::optional<Piece>>> getBoard() {
        return board;
    }

    void to_json(json& j); 

private:
    int rows_, cols_;
    std::vector<std::vector<std::optional<Piece>>> board;
};

#endif // CHESSBOARD_H