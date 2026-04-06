/*
* MIT License

* Copyright (c) [year] [fullname]

* Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
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
    void calculateRepeatMoves();

  private:
    int rows_, cols_;
    std::vector<std::vector<std::optional<Piece>>> board;
};

#endif // CHESSBOARD_H
