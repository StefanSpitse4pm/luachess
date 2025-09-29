export interface piece {
        position: { row: number; col: number };
        image: string;
        type: string;
        possibleMoves: { dx: number; dy: number; repeat: boolean, basedOnlastMove: boolean }[];
        possibleTakes?: { dx: number; dy: number;}[]; // the reason this exists is because this should be calculated on the server.
        canJumpOverPieces?: boolean;
    }
