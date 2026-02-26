"use client";
import React, { useState, useEffect  } from 'react';
import Piece from './piece';
import { piece } from '../types/piece';
import { useWebSocketContext } from '../context/WebSocketContext';

export default function Chessboard() {

    const { sendMessage, lastMessage } = useWebSocketContext();
    const [possibleMoves, setPossibleMoves] = useState(new Set<string>());
    const [isPieceSelected, setIsPieceSelected] = useState(true);
    const [selectedPiece, setSelectedPiece] = useState<piece | null>(null);
    const size = 8;

    const [chessboard, setChessboard] = useState<(piece | null)[][]>(() => {
        const initialBoard = Array.from({ length: size }, () => Array(size).fill(null));
        return initialBoard;
    });


    useEffect(() => {
        if (lastMessage) {
            setChessboard(prevBoard => {
                const newBoard = prevBoard.map(row => [...row]);
                lastMessage.forEach((element: any) => {
                    newBoard[element.position.row][element.position.col] = element;
                });
                return newBoard;
            });
        }
    }, [lastMessage])



    function handleSquareClick(piecePosition: piece | null) {
        const newPosibleMoves = new Set<string>();
        isPieceSelected ? setIsPieceSelected(false) : setIsPieceSelected(true);
        if (piecePosition && isPieceSelected) {
            setSelectedPiece(piecePosition);
            let lastdx = 0;
            let lastdy = 0;
            piecePosition.possibleMoves.forEach(move => {
                const r = piecePosition.position.row + move.dy + (move.basedOnLastMove ? lastdy : 0);
                const c = piecePosition.position.col + move.dx + (move.basedOnLastMove ? lastdx : 0);
                if (r < 0 || r >= size || c < 0 || c >= size) return;

                newPosibleMoves.add(`${r},${c}`);
                if (move.repeat) {
                    let repeatRow = r;
                    let repeatCol = c;
                    while (repeatRow >= 0 && repeatRow < size && repeatCol >= 0 && repeatCol < size) {
                        if (chessboard[repeatRow][repeatCol] !== null && !piecePosition.canJumpOverPieces) break;
                        newPosibleMoves.add(`${repeatRow},${repeatCol}`);
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
                    newPosibleMoves.add(`${r},${c}`);
                }
            });
            setPossibleMoves(newPosibleMoves);
        }
        return;

    }


    function movePiece(moveTo: { row: number; col: number }) {
        console.log(`Moving piece to: ${moveTo.row}, ${moveTo.col}`);
        if (selectedPiece) {
            const { row, col } = selectedPiece.position;

            if (chessboard[moveTo.row][moveTo.col] !== null && selectedPiece.possibleTakes) {
                chessboard[moveTo.row][moveTo.col] = null;
                selectedPiece.possibleTakes = selectedPiece.possibleTakes.filter((take) => !(moveTo.row === take.dy) && moveTo.col === take.dx);
            }

            sendMessage({type: "ChessboardState", payload:{action: "", from: {row, col}, to: {row: moveTo.row, col: moveTo.col}}})

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
