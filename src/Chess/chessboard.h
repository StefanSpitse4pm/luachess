#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "Piece.h"

#include <array>
#include <functional>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

using json = nlohmann::json;

class Chessboard
{
    struct repeatingMoves;
    repeatingMoves* repeatMove_;

    public:
        Chessboard(int rows, int cols)
            : rows_(rows), cols_(cols), board(rows, std::vector<std::optional<Piece>>(cols, std::nullopt))
        {
        }

        [[nodiscard]] bool isOccupied(int row, int col) const
        {
            if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
            {
                return false;
            }
            return board[row][col].has_value();
        }

        std::optional<Piece>& getPieceAt(int row, int col)
        {
            if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
            {
                throw std::out_of_range("Position out of bounds");
            }
            return board[row][col];
        }

        void setPieceAt(const Piece& piece)
        {
            board[piece.position[0]][piece.position[1]] = piece;
        }

        void movePiece(int fromRow, int fromCol, int toRow, int toCol)
        {
            if (fromRow < 0 || fromRow >= rows_ || fromCol < 0 || fromCol >= cols_)
            {
                throw std::out_of_range("Position out of bounds");
            }
            board[toRow][toCol] = board[fromRow][fromCol];
            board[fromRow][fromCol] = std::nullopt;

            board[toRow][toCol]->position[0] = toRow;
            board[toRow][toCol]->position[1] = toCol;
        }

        void foreachPiece(const std::function<void(Piece&)>& func)
        {
            for (int r = 0; r < rows_; ++r)
            {
                for (int c = 0; c < cols_; ++c)
                {
                    if (board[r][c].has_value())
                    {
                        func(board[r][c].value());
                    }
                }
            }
        }

        std::vector<std::vector<std::optional<Piece>>> getBoard()
        {
            return board;
        }

        int getRows() const
        {
            return rows_;
        }

        int getCols() const
        {
            return cols_;
        }

        json toJson();
        void unrollRepeatMoves();
        void repeatUntilBlocked(const std::vector<Move>& moves, Piece& piece) const;
        repeatingMoves findRepeatingMoves(const std::vector<Move>&, repeatingMoves& repeat);
        std::vector<Move> removeRepeatingMoves(std::vector<Move>& moves, repeatingMoves& repeat);

      private:
        int rows_, cols_;
        std::vector<std::vector<std::optional<Piece>>> board;
    };

#endif // CHESSBOARD_H
