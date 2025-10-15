#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <optional>
#include <array>

struct Move {
    int dx;
    int dy;
    bool repeat = false;
    bool basedOnLastMove = false;
};

struct Piece {
    std::array<int, 2> position;   // position[0] = row, position[1] = col
    std::string type;
    std::string image;
    std::vector<Move> possibleMoves;
    std::vector<Move> possibleTakes;
    bool canJumpOverPieces = false;
    std::string color;

    void addMove(int dx, int dy, bool repeat = false, bool basedOnLastMove = false) {
        possibleMoves.push_back({dx, dy, repeat, basedOnLastMove});
    }

    void addTake(int dx, int dy, bool repeat = false, bool basedOnLastMove = false) {
        possibleTakes.push_back({dx, dy, repeat, basedOnLastMove});
    }

    void clearMoves() {
        possibleMoves.clear();
        possibleTakes.clear();
    }

    void foreachMove(const std::function<void(Move&)>& func) {
        for (Move& move : possibleMoves) {
            func(move);
        }
        for (Move& take : possibleTakes) {
            func(take);
        }
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
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
            throw std::out_of_range("Position out of bounds");
        }
        return board[row][col];
            
    }

    void setPieceAt(const Piece& piece) {
        board[piece.position[0]][piece.position[1]] = piece;
    }

    void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        if (fromRow < 0 || fromRow >= rows_ || fromCol < 0 || fromCol >= cols_) {
            throw std::out_of_range("Position out of bounds");
        }
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = std::nullopt;

        board[toRow][toCol]->position[0] = toRow;
        board[toRow][toCol]->position[1] = toCol;
    }

    void foreachPiece(const std::function<void(Piece&)>& func) {
        for (int r = 0; r < rows_; ++r) {
            for (int c = 0; c < cols_; ++c) {
                if (board[r][c].has_value()) {
                    func(board[r][c].value());
                }
            }
        }

    }

    void calculateRepeatMoves() {
        foreachPiece([this](Piece& piece) {
            piece.foreachMove([this, &piece](Move& move) {
                if (move.repeat) {
                    int step = 1;
                    while (true) {
                        int newRow = piece.position[0] + move.dy * step;
                        int newCol = piece.position[1] + move.dx * step;
                        if (newRow < 0 || newRow >= rows_ || newCol < 0 || newCol >= cols_) {
                            break; // Out of bounds
                        }
                        if (isOccupied(newRow, newCol)) {
                            break; // Blocked by another piece
                        }
                        step++;
                    }
                    move.dx *= (step - 1);
                    move.dy *= (step - 1);
                }
            });
        });
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