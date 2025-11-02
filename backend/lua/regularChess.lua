function takeOnDiagonals(piece, board)
    local dirs = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} }
    for _, d in ipairs(dirs) do
        local dx, dy = d[1], d[2]
        local step = 1
        while true do
            local newRow = piece.position[1] + dy * step
            local newCol = piece.position[2] + dx * step
            if newRow < 0 or newRow >= board.rows or newCol < 0 or newCol >= board.cols then
                break
            end
            if board:isOccupied(newRow, newCol) then
                local p = board:getPieceAt(newRow, newCol)
                if p.color ~= piece.color then
                    piece:addTake(dx * step, dy * step, false, false)
                end
                break
            end
            step = step + 1
        end
    end
    return piece
end

function takeStraight(piece, board)
    local dirs = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} }
    for _, d in ipairs(dirs) do
        local dx, dy = d[1], d[2]
        for step = 1, math.max(board.rows, board.cols) do
            local newRow = piece.position[1] + dy * step
            local newCol = piece.position[2] + dx * step
            if newRow < 0 or newRow >= board.rows or newCol < 0 or newCol >= board.cols then
                break
            end
            if board:isOccupied(newRow, newCol) then
                local p = board:getPieceAt(newRow, newCol)
                if p.color ~= piece.color then
                    piece:addTake(dx * step, dy * step, false, false)
                end
                break
            end
        end
    end
    return piece
end

local pieces = {
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
    rook = {
        get_moves = function(piece, board)
            piece:clearMoves()
            piece:addMove(0, -1, true, false)  
            piece:addMove(-1, 0, true, false) 
            piece:addMove(0, 1, true, false)  
            piece:addMove(1, 0, true, false)
            piece = takeStraight(piece, board)            
            print(piece)
            board:calculateRepeatMoves(piece)
            board:setPieceAt(piece)
        end
    },
    knight = {
        get_moves = function(piece, board) 
            piece:clearMoves()
            local moves = {
                {1, 2}, {2, 1}, {-1, 2}, {-2, 1},
                {1, -2}, {2, -1}, {-1, -2}, {-2, -1}
            }
            for _, move in ipairs(moves) do
                local newRow = piece.position[1] + move[2]
                local newCol = piece.position[2] + move[1]
                if newRow >= 0 and newRow < board.rows and newCol >= 0 and newCol < board.cols then
                    if not board:isOccupied(newRow, newCol) then
                        piece:addMove(move[1], move[2], false, false)
                    elseif board:getPieceAt(newRow, newCol).color ~= piece.color then
                        piece:addTake(move[1], move[2], false, false)
                    end
                end
            end
            board:setPieceAt(piece)
        end
    },
    bishop = {
        get_moves = function(piece, board) 
            piece:clearMoves()
            piece:addMove(-1, -1, true, false)
            piece:addMove(-1, 1, true, false)
            piece:addMove(1, -1, true, false)
            piece:addMove(1, 1, true, false)
            piece = takeOnDiagonals(piece, board)
            board:setPieceAt(piece)
        end
    },
    queen = {
        get_moves = function(piece, board)
            piece:clearMoves()
            piece:addMove(0, -1, true, false)  
            piece:addMove(-1, 0, true, false) 
            piece:addMove(0, 1, true, false)  
            piece:addMove(1, 0, true, false)
            piece:addMove(-1, -1, true, false)
            piece:addMove(-1, 1, true, false)
            piece:addMove(1, -1, true, false)
            piece:addMove(1, 1, true, false)
            piece = takeOnDiagonals(piece, board)
            piece = takeStraight(piece, board)
            board:setPieceAt(piece)
        end

    },
    king = {
        get_moves = function(piece, board) end
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
    -- for x = 0, board.rows - 1 do
    --     board:setPieceAt(createPiece("pawn", "Chess_plt45.svg", x, 6, "white"))
    --     board:setPieceAt(createPiece("pawn", "Chess_pdt45.svg", x, 1, "black"))
    -- end
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
    -- board:setPieceAt(createPiece("queen", "Chess_qlt45.svg", 3, 7, "white"))
    -- board:setPieceAt(createPiece("queen", "Chess_qdt45.svg", 3, 0, "black"))
    -- board:setPieceAt(createPiece("king", "Chess_klt45.svg", 4, 7, "white"))
    -- board:setPieceAt(createPiece("king", "Chess_kdt45.svg", 4, 0, "black"))

    getLegalMoves(board)
end