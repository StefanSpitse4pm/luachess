#include "chessboard.h"
#include <iostream>

void Chessboard::to_json(json& j)
{
    j = json::array();
    for (int r = 0; r < rows_; ++r)
    {
        for (int c = 0; c < cols_; ++c)
        {
            if (board[r][c].has_value())
            {
                const Piece& piece = board[r][c].value();
                json pieceJson;
                pieceJson["position"] = {{"row", r}, {"col", c}};
                pieceJson["type"] = piece.type;
                pieceJson["canJumpOverPieces"] = piece.canJumpOverPieces;
                pieceJson["image"] = piece.image;

                pieceJson["possibleMoves"] = json::array();
                for (const auto& move : piece.possibleMoves)
                {
                    pieceJson["possibleMoves"].push_back(
                        {{"dx", move.dx},
                         {"dy", move.dy},
                         {"repeat", move.repeat},
                         {"basedOnLastMove", move.basedOnLastMove}}
                    );
                }

                pieceJson["possibleTakes"] = json::array();
                for (const auto& take : piece.possibleTakes)
                {
                    pieceJson["possibleTakes"].push_back(
                        {{"dx", take.dx},
                         {"dy", take.dy},
                         {"repeat", take.repeat},
                         {"basedOnLastMove", take.basedOnLastMove}}
                    );
                }

                j.push_back(pieceJson);
            }
        }
    }
}

void Chessboard::calculateRepeatMoves()
{
    foreachPiece(
        [this](Piece& piece)
        {
            std::vector<size_t> repeatMovesIndices;
            std::vector<size_t> repeatTakesIndices;
            std::vector<Move> repeatMovesToExpand;
            std::vector<Move> repeatTakesToExpand;

            for (size_t i = 0; i < piece.possibleMoves.size(); ++i)
            {
                if (piece.possibleMoves[i].repeat)
                {
                    repeatMovesToExpand.push_back(piece.possibleMoves[i]);
                    repeatMovesIndices.push_back(i);
                }
            }
            for (size_t i = 0; i < piece.possibleTakes.size(); ++i)
            {
                if (piece.possibleTakes[i].repeat)
                {
                    repeatTakesToExpand.push_back(piece.possibleTakes[i]);
                    repeatTakesIndices.push_back(i);
                }
            }

            for (auto it = repeatMovesIndices.rbegin(); it != repeatMovesIndices.rend(); ++it)
            {
                piece.possibleMoves.erase(piece.possibleMoves.begin() + *it);
            }
            for (auto it = repeatTakesIndices.rbegin(); it != repeatTakesIndices.rend(); ++it)
            {
                piece.possibleTakes.erase(piece.possibleTakes.begin() + *it);
            }

            for (const Move& repeatMove : repeatMovesToExpand)
            {
                int step = 1;
                while (true)
                {
                    int newRow = piece.position[0] + repeatMove.dy * step;
                    int newCol = piece.position[1] + repeatMove.dx * step;
                    if (newRow < 0 || newRow >= rows_ || newCol < 0 || newCol >= cols_)
                    {
                        break; // Out of bounds
                    }
                    if (isOccupied(newRow, newCol))
                    {
                        break; // Blocked by another piece
                    }
                    piece.possibleMoves.push_back(
                        {repeatMove.dx * step, repeatMove.dy * step, false, repeatMove.basedOnLastMove}
                    );
                    step++;
                }
            }

            // Expand repeat takes
            for (const Move& repeatTake : repeatTakesToExpand)
            {
                int step = 1;
                while (true)
                {
                    int newRow = piece.position[0] + repeatTake.dy * step;
                    int newCol = piece.position[1] + repeatTake.dx * step;
                    if (newRow < 0 || newRow >= rows_ || newCol < 0 || newCol >= cols_)
                    {
                        break; // Out of bounds
                    }
                    if (isOccupied(newRow, newCol))
                    {
                        // If there's a piece at this position, we can potentially take it
                        // (the game logic will determine if it's an enemy piece)
                        piece.possibleTakes.push_back(
                            {repeatTake.dx * step, repeatTake.dy * step, false, repeatTake.basedOnLastMove}
                        );
                        break; // Stop after finding a piece (can't continue past it)
                    }
                    // If no piece at this position, we can't take here, but continue looking
                    step++;
                }
            }
        }
    );
}
