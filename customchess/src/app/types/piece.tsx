export interface piece {
        position: { row: number; col: number };
        image: string;
        type: string;
        posibleMoves: { dx: number; dy: number; repeat: boolean, basedOnlastMove: boolean }[];
        posibleTakes?: { dx: number; dy: number;}[]; // the reason this exists is because this should be calculated on the server.
        canJumpOverPieces?: boolean;
    }
