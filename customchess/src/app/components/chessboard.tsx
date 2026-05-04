"use client";
import React, {useEffect, useMemo, useRef, useState} from 'react';
import Piece from './piece';
import {piece} from '../types/piece';
import {useWebSocketContext} from '../context/WebSocketContext';

type ChessboardProps = {
    gameId: number | undefined;
    publicId: string | undefined;
    username: string | undefined;
};

export default function Chessboard({ gameId, publicId, username }: ChessboardProps) {

    const { sendMessage, lastMessage } = useWebSocketContext();
    const [possibleMoves, setPossibleMoves] = useState(new Set<string>());
    const [isPieceSelected, setIsPieceSelected] = useState(true);
    const [selectedPiece, setSelectedPiece] = useState<piece | null>(null);
    const [activePlayer, setActivePlayer] = useState<string | null>(null);
    const size = 8;

    // Avoid re-requesting board state on every incoming message.
    const requestedGameIdRef = useRef<number | null>(null);

    const [chessboard, setChessboard] = useState<(piece | null)[][]>(() => {
        const initialBoard = Array.from({ length: size }, () => Array(size).fill(null));
        return initialBoard;
    });

    const movesEnabled = useMemo(() => {
        if (typeof gameId !== 'number') return false;
        if (!publicId || publicId.length === 0) return false;
        if (!username) return false;
        // Be conservative: if server didn't provide an active player yet, disallow moves.
        if (!activePlayer) return false;
        return username === activePlayer;
    }, [activePlayer, gameId, publicId, username]);


    useEffect(() => {
        if (typeof gameId !== 'number') return;
        if (requestedGameIdRef.current === gameId) return;
        sendMessage({ type: "Game", payload: {"action":"boardState", "gameId":gameId} });
        requestedGameIdRef.current = gameId;
    }, [gameId, sendMessage]);

    useEffect(() => {
        if (!lastMessage) return;

        // Ignore messages for other games when an id is present.
        const msgGameId = (lastMessage as any)?.gameId ?? (lastMessage as any)?.id ?? (lastMessage as any)?.payload?.gameId;
        if (typeof msgGameId === 'number' && typeof gameId === 'number' && msgGameId !== gameId) {
            return;
        }

        const nextActivePlayer = (lastMessage as any)?.activePlayer ?? (lastMessage as any)?.payload?.activePlayer;
        if (typeof nextActivePlayer === 'string') {
            setActivePlayer(nextActivePlayer);
        }

        const boardData = Array.isArray(lastMessage)
            ? lastMessage
            : Array.isArray((lastMessage as any)?.board)
                ? (lastMessage as any).board
                : Array.isArray((lastMessage as any)?.payload?.board)
                    ? (lastMessage as any).payload.board
                    : null;



        if (!boardData) {
            return;
        }


        setChessboard(Array.from({ length: size }, () => Array(size).fill(null)));
        setChessboard(prevBoard => {
            const newBoard = prevBoard.map(row => [...row]);
            boardData.forEach((element: any) => {
                if (
                    element?.position &&
                    Number.isInteger(element.position.row) &&
                    Number.isInteger(element.position.col)
                ) {
                    newBoard[element.position.row][element.position.col] = element;
                }
            });
            return newBoard;
        });
    }, [lastMessage, gameId])

    // If the turn changes away from us, clear any in-progress selection/highlights.
    useEffect(() => {
        if (!movesEnabled) {
            setSelectedPiece(null);
            setPossibleMoves(new Set<string>());
            setIsPieceSelected(true);
        }
    }, [movesEnabled]);



    function handleSquareClick(piecePosition: piece | null) {
        if (!movesEnabled) return;
        const newPossibleMoves = new Set<string>();
        isPieceSelected ? setIsPieceSelected(false) : setIsPieceSelected(true);
        if (piecePosition && isPieceSelected) {
            setSelectedPiece(piecePosition);
            let lastdx = 0;
            let lastdy = 0;
            piecePosition.possibleMoves.forEach(move => {
                const r = piecePosition.position.row + move.dy + (move.basedOnLastMove ? lastdy : 0);
                const c = piecePosition.position.col + move.dx + (move.basedOnLastMove ? lastdx : 0);
                if (r < 0 || r >= size || c < 0 || c >= size) return;

                newPossibleMoves.add(`${r},${c}`);
                if (move.repeat) {
                    let repeatRow = r;
                    let repeatCol = c;
                    while (repeatRow >= 0 && repeatRow < size && repeatCol >= 0 && repeatCol < size) {
                        if (chessboard[repeatRow][repeatCol] !== null && !piecePosition.canJumpOverPieces) break;
                        newPossibleMoves.add(`${repeatRow},${repeatCol}`);
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

                // Validate bounds
                if (r >= 0 && r < size && c >= 0 && c < size) {
                    newPossibleMoves.add(`${r},${c}`);
                }
            });
            setPossibleMoves(newPossibleMoves);
        }
        return;

    }


    function movePiece(moveTo: { row: number; col: number }) {
        if (!movesEnabled) return;
        console.log(`Moving piece to: ${moveTo.row}, ${moveTo.col}`);
        if (selectedPiece) {
            const { row, col } = selectedPiece.position;

            if (chessboard[moveTo.row][moveTo.col] !== null && selectedPiece.possibleTakes) {
                chessboard[moveTo.row][moveTo.col] = null;
                selectedPiece.possibleTakes = selectedPiece.possibleTakes.filter((take) => !(moveTo.row === take.dy) && moveTo.col === take.dx);
            }

            sendMessage({type: "Game", payload:{action: "move", move: {fromRow: row, fromCol: col, toRow: moveTo.row, toCol: moveTo.col, }, "gameId":gameId, "pid":publicId}})

            setChessboard(prevBoard => {
                const newBoard = prevBoard.map(row => [...row]);
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
            {activePlayer && username && (
                <div className="mb-4 text-gray-200">
                    Active player: <span className={movesEnabled ? "text-green-400 font-semibold" : "text-yellow-300 font-semibold"}>{activePlayer}</span>
                    {!movesEnabled && <span className="ml-2 text-sm text-gray-400">(waiting for your turn)</span>}
                </div>
            )}
           {Array.from({ length: size }).map((_, row) => (
                <div key={row} className="flex">
                    {Array.from({ length: size }).map((_, col) => {
                        const isWhite = (row + col) % 2 === 0;
                        const posKey = `${row},${col}`;
                        const isHighlighted = movesEnabled && possibleMoves.has(posKey) && !isPieceSelected;
                        return (
                            <div
                                key={col}
                                className={`chessboard-square-${row}-${col} ${isWhite ? 'bg-white' : 'bg-purple-900'} w-24 h-24 flex relative items-center justify-center`}
                                onClick={() => handleSquareClick(chessboard[row][col])}
                            >
                                {chessboard[row][col] ? (
                                    <Piece image={chessboard[row][col].image} position={chessboard[row][col].position} type={chessboard[row][col].type} possibleMoves={chessboard[row][col].possibleMoves} chessboard={chessboard} color={chessboard[row][col].color} />
                                ) : (
                                    <div className={`w-8 h-8 rounded-full bg-gray-400 opacity-40 ${isHighlighted ? 'block' : 'hidden'}`} onClick={() => movePiece({ row, col })}></div>
                                )}
                                {chessboard[row][col] && possibleMoves.has(posKey) && (
                                    <div className={`w-24 h-24 rounded-full bg-transparent border-gray-500 border-7 opacity-40 z-20 absolute ${isHighlighted ? 'block' : 'hidden'}`} onClick={() => movePiece({ row, col })}></div>
                                )}

                            </div>
                        );
                    })}
                </div>
            ))}
        </div>
    )
}
