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

    void addMove(int dx, int dy, bool repeat = false, bool basedOnLastMove = false) {
        possibleMoves.push_back({dx, dy, repeat, basedOnLastMove});
    }

    void addTake(int dx, int dy, bool repeat = false, bool basedOnLastMove = false) {
        possibleTakes.push_back({dx, dy, repeat, basedOnLastMove});
    }

};

using json = nlohmann::json;

class Chessboard {
public:
    Chessboard(int rows, int cols) : rows_(rows), cols_(cols), board(rows, std::vector<std::optional<Piece>>(cols, std::nullopt)) {}

    bool isOccupied(int row, int col) const {
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
            return false;
        }
        return board[row][col].has_value();
    }


    std::optional<Piece>& getPieceAt(int row, int col) {
        return board[row][col];
    }

    void setPieceAt(const Piece& piece) {
        board[piece.position[0]][piece.position[1]] = piece;
    }

    void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = Piece();
    }
    std::vector<std::vector<std::optional<Piece>>> getBoard() {
        return board;
    }

    int getRows() const { return rows_; }
    int getCols() const { return cols_; }

    void to_json(json& j); 

private:
    int rows_, cols_;
    std::vector<std::vector<std::optional<Piece>>> board;
};

#endif // CHESSBOARD_H