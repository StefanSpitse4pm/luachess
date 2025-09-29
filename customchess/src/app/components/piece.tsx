"use client";
import { piece } from '../types/piece';

type PieceProps = piece & {
    chessboard: (piece | null)[][];
}

export default function Piece({ position, image, possibleMoves: posibleMoves, chessboard }: PieceProps) {
    return (
        <div className={`piece w-24 h-24 relative z-10 `}
             style={{ backgroundImage: `url(${image})`,
             backgroundRepeat: 'no-repeat', backgroundPosition: `50% 50%`,
             backgroundSize: '100% 100%'}}>

        </div>
    );
}


    

