pieces = {
    pawn = {
        get_moves = function(piece, board)
            piece:clearMoves()
            local dir = piece.color == "white" and -1 or 1
            if (board:isOccupied(piece.position[1] + dir, piece.position[2]) == false) then
                piece:addMove(0, dir, false, false)
                print(piece.position[1] + 2)
                if (piece.position[1] == 6 and piece.color == "white" or piece.position[1] == 1 and piece.color == "black" and board:isOccupied(piece.position[1], piece.position[2] + 2) == false) then
                    piece:addMove(0,  1 * dir, false, true)
                end
            end
            if (board:isOccupied(piece.position[1] + dir, piece.position[2] - 1) and board:getPieceAt(piece.position[1] + dir, piece.position[2] - 1).color ~= piece.color) then
                piece:addTake(-1, dir, false, false)
            end
            if (board:isOccupied(piece.position[1] + dir, piece.position[2] + 1) and board:getPieceAt(piece.position[1] + dir, piece.position[2] + 1).color ~= piece.color) then
                piece:addTake(1, dir, false, false)
            end

            board:setPieceAt(piece)
        end
    },
    rook = {get_moves = function(piece, board) end},
    knight = {get_moves = function(piece, board) end},
    bishop = {get_moves = function(piece, board) end},
    queen = {get_moves = function(piece, board) end},
    king = {get_moves = function(piece, board) end}
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
    for x = 0, board.rows - 1 do
        board:setPieceAt(createPiece("pawn", "Chess_plt45.svg", x, 6, "white"))
        board:setPieceAt(createPiece("pawn", "Chess_pdt45.svg", x, 1, "black"))
    end
    board:setPieceAt(createPiece("rook", "Chess_rlt45.svg", 0, 7, "white"))
    board:setPieceAt(createPiece("rook", "Chess_rlt45.svg", 7, 7, "white"))
    board:setPieceAt(createPiece("rook", "Chess_rdt45.svg", 0, 0, "black"))
    board:setPieceAt(createPiece("rook", "Chess_rdt45.svg", 7, 0, "black"))
    board:setPieceAt(createPiece("knight", "Chess_nlt45.svg", 1, 7, "white"))
    board:setPieceAt(createPiece("knight", "Chess_nlt45.svg", 6, 7, "white"))
    board:setPieceAt(createPiece("knight", "Chess_ndt45.svg", 1, 0, "black"))
    board:setPieceAt(createPiece("knight", "Chess_ndt45.svg", 6, 0, "black"))
    board:setPieceAt(createPiece("bishop", "Chess_blt45.svg", 2, 7, "white"))
    board:setPieceAt(createPiece("bishop", "Chess_blt45.svg", 5, 7, "white"))
    board:setPieceAt(createPiece("bishop", "Chess_bdt45.svg", 2, 0, "black"))
    board:setPieceAt(createPiece("bishop", "Chess_bdt45.svg", 5, 0, "black"))
    board:setPieceAt(createPiece("queen", "Chess_qlt45.svg", 3, 7, "white"))
    board:setPieceAt(createPiece("queen", "Chess_qdt45.svg", 3, 0, "black"))
    board:setPieceAt(createPiece("king", "Chess_klt45.svg", 4, 7, "white"))
    board:setPieceAt(createPiece("king", "Chess_kdt45.svg", 4, 0, "black"))

    getLegalMoves(board)
end