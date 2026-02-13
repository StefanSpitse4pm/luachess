---@diagnostic disable: undefined-doc-param, undefined-doc-name

---@class Move
---@field dx number The horizontal displacement
---@field dy number The vertical displacement
---@field repeat boolean Whether this move can be repeated
---@field basedOnLastMove boolean Whether this move is based on the last move

---@class Piece
---@field position number[] Position array where [1] = row, [2] = col (Lua 1-indexed)
---@field type string The type of the piece (e.g., "pawn", "rook", "knight", "bishop", "queen", "king")
---@field image string The image filename for this piece (e.g., "Chess_plt45.svg")
---@field possibleMoves Move[] Array of possible moves for this piece
---@field possibleTakes Move[] Array of possible taking moves for this piece
---@field canJumpOverPieces boolean Whether this piece can jump over other pieces
---@field color string The color of the piece ("white" or "black")

---Add a possible move to this piece
---@param dx number Horizontal displacement
---@param dy number Vertical displacement
---@param repeat? boolean Whether this move can be repeated (default: false)
---@param basedOnLastMove? boolean Whether this move is based on the last move (default: false)
function Piece:addMove(dx, dy, repeating, basedOnLastMove) end

---Add a possible take to this piece
---@param dx number Horizontal displacement
---@param dy number Vertical displacement
---@param repeat? boolean Whether this take can be repeated (default: false)
---@param basedOnLastMove? boolean Whether this take is based on the last move (default: false)
function Piece:addTake(dx, dy, repeating, basedOnLastMove) end

---Clear all moves and takes for this piece
function Piece:clearMoves() end

---@class Chessboard
---@field rows number The number of rows on the chessboard
---@field cols number The number of columns on the chessboard

---Check if a position on the board is occupied
---@param row number The row to check (0-indexed from C++ side)
---@param col number The column to check (0-indexed from C++ side)
---@return boolean occupied True if the position is occupied
function Chessboard:isOccupied(row, col) end

---Get the piece at a specific position
---@param row number The row (0-indexed from C++ side)
---@param col number The column (0-indexed from C++ side)
---@return Piece|nil piece The piece at the position, or nil if empty
function Chessboard:getPieceAt(row, col) end

---Set a piece at its position on the board
---@param piece Piece The piece to place on the board (position is taken from piece.position)
function Chessboard:setPieceAt(piece) end

---Move a piece from one position to another
---@param fromRow number The starting row (0-indexed from C++ side)
---@param fromCol number The starting column (0-indexed from C++ side)
---@param toRow number The destination row (0-indexed from C++ side)
---@param toCol number The destination column (0-indexed from C++ side)
function Chessboard:movePiece(fromRow, fromCol, toRow, toCol) end

---Calculate and expand repeat moves for all pieces on the board
---This expands moves marked with repeat=true into concrete move positions
function Chessboard:calculateRepeatMoves() end

---Create a new piece
---@param type string The type of the piece (e.g., "pawn", "rook", "knight", "bishop", "queen", "king")
---@param image string The image filename for this piece (e.g., "Chess_plt45.svg")
---@param row number The initial row position (0-indexed from C++ side)
---@param col number The initial column position (0-indexed from C++ side)
---@param color string The color of the piece ("white" or "black")
---@return Piece piece The newly created piece
function createPiece(type, image, row, col, color) end
