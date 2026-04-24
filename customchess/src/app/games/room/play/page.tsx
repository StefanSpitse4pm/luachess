"use client";
import { useEffect } from 'react';
import { useRouter, useSearchParams } from 'next/navigation';
import Chessboard from '../../../components/chessboard';

export default function Play() {
    const router = useRouter();
    const searchParams = useSearchParams();
    const username = searchParams.get('username');
    const gameId = searchParams.get('game');
    const publicId = searchParams.get('publicId')


    useEffect(() => {
        if (!username) {
            router.push('/games');
        }
    }, [username, router]);

    const parseGameId = gameId ? parseInt(gameId) : undefined;
    return (
        <main className="flex min-h-screen flex-col items-center justify-center p-24">
            <p className="text-lg text-gray-300 mb-4">Playing as: {username}</p>
            {Chessboard(parseGameId, publicId)}
        </main>
    );
}
