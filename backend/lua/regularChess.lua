pieces = {
    pawn = {
        get_moves = function(piece, board)
            local dir = piece.color == "white" and -1 or 1
            if (board:isOccupied(piece.position[1] + dir, piece.position[2]) == false) then
                piece:addMove(0, dir, false, true)
                if (piece.position[1] == (piece.color == "white" and 6 or 1) and board:isOccupied(piece.position[1] + 2 * dir, piece.position[2]) == false) then
                    piece:addMove(0, 2 * dir, false, true)
                end
            end

        end
    }
}

function getLegalMoves(board)
    for x = 0, board.rows - 1 do
        for y = 0, board.cols - 1 do
            if board:getPieceAt(x, y) ~= nil then
                pieces[board:getPieceAt(x, y).type].get_moves(board:getPieceAt(x, y), board)
            end
        end
    end
end

function setup(board)
    local piece = createPiece("pawn", "Chess_plt45.svg", 6, 1, "white")
    board:setPieceAt(piece)
    getLegalMoves(board)
end