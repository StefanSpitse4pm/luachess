"use client";
import { piece } from '../types/piece';

type PieceProps = piece & {
    chessboard: (piece | null)[][];
}

function normalizePublicAssetPath(path: string): string {
    const trimmed = (path ?? '').trim();
    if (!trimmed) return '';

    // Next.js serves `/public/*` at the site root, so `public/foo.svg` -> `/foo.svg`.
    const withoutPublicPrefix = trimmed.replace(/^\/?public\//, '');

    // Ensure root-relative so it doesn't become relative to the current route.
    return withoutPublicPrefix.startsWith('/') ? withoutPublicPrefix : `/${withoutPublicPrefix}`;
}

export default function Piece({ position, image, possibleMoves: posibleMoves, chessboard }: PieceProps) {
    const src = normalizePublicAssetPath(image);

    return (
        <div
            className="piece w-24 h-24 relative z-10"
            style={{
                backgroundImage: src ? `url(\"${src}\")` : undefined,
                backgroundRepeat: 'no-repeat',
                backgroundPosition: '50% 50%',
                backgroundSize: '100% 100%',
            }}
        />
    );
}
