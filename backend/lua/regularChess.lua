pieces = {
    pawn = {
        get_moves = function(piece, board)
            piece:clearMoves()
            local dir = piece.color == "white" and -1 or 1
            if (board:isOccupied(piece.position[1] + dir, piece.position[2]) == false) then
                piece:addMove(0, dir, false, false)
                if (piece.position[1] == 6 and piece.color == "white" or piece.position[1] == 1 and piece.color == "black") then
                    piece:addMove(0,  1 * dir, false, true)
                end
            end
            board:setPieceAt(piece)
        end
    }
}

function getLegalMoves(board)
    print("Getting legal moves for all pieces on the board")
    for x = 0, board.rows - 1 do
        for y = 0, board.cols - 1 do
            if board:getPieceAt(x, y) ~= nil then
                pieces[board:getPieceAt(x, y).type].get_moves(board:getPieceAt(x, y), board)
            end
        end
    end
end

function setup(board)
    local piece = createPiece("pawn", "Chess_plt45.svg", 1, 6, "white")
    board:setPieceAt(piece)
    local piece = createPiece("pawn", "Chess_pdt45.svg", 1, 1, "black")
    board:setPieceAt(piece)
    getLegalMoves(board)
end