#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

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
    Chessboard(int rows, int cols) : rows(rows), cols(cols) {
        board.resize(rows, std::vector<Piece>(cols));
    }

    Piece& getPieceAt(int row, int col) {
        return board[row][col];
    }

    void setPieceAt(int row, int col, const Piece& piece) {
        board[row][col] = piece;
    }

    void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = Piece();
    }

    void to_json(json& j); 
    

private:
    int rows, cols;
    std::vector<std::vector<Piece>> board;
};

#endif // CHESSBOARD_H