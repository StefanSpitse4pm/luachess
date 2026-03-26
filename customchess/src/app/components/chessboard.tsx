"use client";
import React, {useEffect, useState} from 'react';
import Piece from './piece';
import {piece} from '../types/piece';
import {useWebSocketContext} from '../context/WebSocketContext';

type ChessboardProps = {
    gameId?: number;
};

type ServerMove = {
    fromRow: number;
    fromCol: number;
    toRow: number;
    toCol: number;
};

function toPosKey(row: number, col: number) {
    return `${row},${col}`;
}

function normalizeMoveMessage(msg: any): {
    lastMove: ServerMove;
    possibleMoves?: any[];
    possibleTakes?: any[];
} | null {
    if (!msg) return null;
    const root = msg?.payload && typeof msg.payload === 'object' ? msg.payload : msg;
    if (root?.type !== 'move') return null;

    const lastMove = root?.lastMove;
    if (
        !lastMove ||
        !Number.isInteger(lastMove.fromRow) ||
        !Number.isInteger(lastMove.fromCol) ||
        !Number.isInteger(lastMove.toRow) ||
        !Number.isInteger(lastMove.toCol)
    ) {
        return null;
    }

    return {
        lastMove,
        possibleMoves: Array.isArray(root?.PossibleMoves) ? root.PossibleMoves : undefined,
        possibleTakes: Array.isArray(root?.PossibleTakes) ? root.PossibleTakes : undefined,
    };
}

function extractBoardData(msg: any): any[] | null {
    if (!msg) return null;
    if (Array.isArray(msg)) return msg;
    if (Array.isArray(msg?.board)) return msg.board;
    if (Array.isArray(msg?.payload?.board)) return msg.payload.board;
    return null;
}

function coordsFromAny(entry: any): { row: number; col: number } | null {
    if (!entry) return null;
    // Accept both {row,col} and {position:{row,col}} shapes.
    if (Number.isInteger(entry.row) && Number.isInteger(entry.col)) return { row: entry.row, col: entry.col };
    if (entry.position && Number.isInteger(entry.position.row) && Number.isInteger(entry.position.col)) {
        return { row: entry.position.row, col: entry.position.col };
    }
    return null;
}

export default function Chessboard({ gameId }: ChessboardProps) {

    const { sendMessage, lastMessage } = useWebSocketContext();
    const [possibleMoves, setPossibleMoves] = useState(new Set<string>());
    const [firstLoaded, setFirstLoaded] = useState(false);
    const [isPieceSelected, setIsPieceSelected] = useState(true);
    const [selectedPiece, setSelectedPiece] = useState<piece | null>(null);
    const size = 8;

    const [chessboard, setChessboard] = useState<(piece | null)[][]>(() => {
        const initialBoard = Array.from({ length: size }, () => Array(size).fill(null));
        return initialBoard;
    });


    useEffect(() => {
        if (!firstLoaded) {
            sendMessage({ type: "Game", payload: { action: "boardState", gameId } });
            setFirstLoaded(true);
        }

        // 1) Server move update: apply incrementally.
        const moveMsg = normalizeMoveMessage(lastMessage);
        if (moveMsg) {
            const { lastMove, possibleMoves, possibleTakes } = moveMsg;

            setChessboard(prevBoard => {
                const nextBoard = prevBoard.map(r => r.slice());
                const { fromRow, fromCol, toRow, toCol } = lastMove;
                if (
                    fromRow < 0 || fromRow >= size ||
                    fromCol < 0 || fromCol >= size ||
                    toRow < 0 || toRow >= size ||
                    toCol < 0 || toCol >= size
                ) {
                    return prevBoard;
                }
                const moving = nextBoard[fromRow][fromCol];
                if (!moving) {
                    return prevBoard;
                }
                // Capture-on-destination is handled naturally by overwrite.
                nextBoard[toRow][toCol] = { ...moving, position: { row: toRow, col: toCol } };
                nextBoard[fromRow][fromCol] = null;
                return nextBoard;
            });

            const nextPossible = new Set<string>();
            possibleMoves?.forEach(m => {
                const c = coordsFromAny(m);
                if (!c) return;
                if (c.row < 0 || c.row >= size || c.col < 0 || c.col >= size) return;
                nextPossible.add(toPosKey(c.row, c.col));
            });
            possibleTakes?.forEach(t => {
                const c = coordsFromAny(t);
                if (!c) return;
                if (c.row < 0 || c.row >= size || c.col < 0 || c.col >= size) return;
                nextPossible.add(toPosKey(c.row, c.col));
            });
            setPossibleMoves(nextPossible);

            // Move was confirmed by server; clear selection/highlights logic.
            setSelectedPiece(null);
            setIsPieceSelected(true);
            return;
        }

        // 2) Full board state snapshot.
        const boardData = extractBoardData(lastMessage);
        if (!boardData) return;

        setChessboard(() => {
            const freshBoard: (piece | null)[][] = Array.from({ length: size }, () => Array(size).fill(null));
            boardData.forEach((element: any) => {
                if (
                    element?.position &&
                    Number.isInteger(element.position.row) &&
                    Number.isInteger(element.position.col)
                ) {
                    freshBoard[element.position.row][element.position.col] = element;
                }
            });
            return freshBoard;
        });
    }, [lastMessage, firstLoaded, gameId, sendMessage]);



    function handleSquareClick(piecePosition: piece | null) {
        // Clicking a piece selects it and shows its computed moves.
        if (piecePosition) {
            const newPossibleMoves = new Set<string>();
            setSelectedPiece(piecePosition);
            setIsPieceSelected(false);
            let lastdx = 0;
            let lastdy = 0;
            piecePosition.possibleMoves.forEach(move => {
                const r = piecePosition.position.row + move.dy + (move.basedOnLastMove ? lastdy : 0);
                const c = piecePosition.position.col + move.dx + (move.basedOnLastMove ? lastdx : 0);
                if (r < 0 || r >= size || c < 0 || c >= size) return;

                newPossibleMoves.add(toPosKey(r, c));
                if (move.repeat) {
                    let repeatRow = r;
                    let repeatCol = c;
                    while (repeatRow >= 0 && repeatRow < size && repeatCol >= 0 && repeatCol < size) {
                        if (chessboard[repeatRow][repeatCol] !== null && !piecePosition.canJumpOverPieces) break;
                        newPossibleMoves.add(toPosKey(repeatRow, repeatCol));
                        repeatRow += move.dy;
                        repeatCol += move.dx;
                    }
                }
                lastdx = move.dx;
                lastdy = move.dy;
            });
            piecePosition.possibleTakes?.forEach(take => {
                const r = piecePosition.position.row + take.dy + (take.basedOnLastMove ? lastdy : 0);
                const c = piecePosition.position.col + take.dx + (take.basedOnLastMove ? lastdx : 0);

                if (r >= 0 && r < size && c >= 0 && c < size) {
                    newPossibleMoves.add(toPosKey(r, c));
                }
            });
            setPossibleMoves(newPossibleMoves);
        } else {
            // Clicking an empty square that's not a target clears selection.
            setSelectedPiece(null);
            setPossibleMoves(new Set<string>());
            setIsPieceSelected(true);
        }

    }


    function movePiece(moveTo: { row: number; col: number }) {
        console.log(`Moving piece to: ${moveTo.row}, ${moveTo.col}`);
        if (selectedPiece) {
            const { row, col } = selectedPiece.position;
            sendMessage({type: "Game", payload:{action: "move", move: {fromRow: row, fromCol: col, toRow: moveTo.row, toCol: moveTo.col}, "gameId":gameId}})

            // Optimistic update; server will re-sync/confirm via type='move' message.
            setChessboard(prevBoard => {
                const newBoard = prevBoard.map(r => r.slice());
                newBoard[moveTo.row][moveTo.col] = { ...selectedPiece, position: moveTo };
                newBoard[row][col] = null;
                return newBoard;
            });
            setSelectedPiece(null);
            setPossibleMoves(new Set<string>());
            setIsPieceSelected(true);
        }

    }

    return (
        <div className="chessboard">
           {Array.from({ length: size }).map((_, row) => (
                <div key={row} className="flex">
                    {Array.from({ length: size }).map((_, col) => {
                        const isWhite = (row + col) % 2 === 0;
                        const posKey = `${row},${col}`;
                        const isHighlighted = possibleMoves.has(posKey) && !isPieceSelected;
                        return (
                            <div
                                key={col}
                                className={`chessboard-square-${row}-${col} ${isWhite ? 'bg-white' : 'bg-purple-900'} w-24 h-24 flex relative items-center justify-center`}
                                onClick={() => handleSquareClick(chessboard[row][col])}
                            >
                                {chessboard[row][col] ? (
                                    <Piece image={chessboard[row][col].image} position={chessboard[row][col].position} type={chessboard[row][col].type} possibleMoves={chessboard[row][col].possibleMoves} chessboard={chessboard} color={chessboard[row][col].color} />
                                ) : (
                                    <div
                                        className={`w-8 h-8 rounded-full bg-gray-400 opacity-40 ${isHighlighted ? 'block' : 'hidden'}`}
                                        onClick={(e) => {
                                            e.stopPropagation();
                                            movePiece({ row, col });
                                        }}
                                    />
                                )}
                                {chessboard[row][col] && possibleMoves.has(posKey) && (
                                    <div
                                        className={`w-24 h-24 rounded-full bg-transparent border-gray-500 border-7 opacity-40 z-20 absolute ${isHighlighted ? 'block' : 'hidden'}`}
                                        onClick={(e) => {
                                            e.stopPropagation();
                                            movePiece({ row, col });
                                        }}
                                    />
                                )}

                            </div>
                        );
                    })}
                </div>
            ))}
        </div>
    )
}
