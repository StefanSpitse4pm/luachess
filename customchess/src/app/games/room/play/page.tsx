"use client";
import { useEffect } from 'react';
import { useRouter, useSearchParams } from 'next/navigation';
import Chessboard from '../../../components/chessboard';

export default function Play() {
    const router = useRouter();
    const searchParams = useSearchParams();
    const roomName = searchParams.get('roomName');
    const username = searchParams.get('username');

    useEffect(() => {
        if (!roomName || !username) {
            router.push('/games');
        }
    }, [roomName, username]);

    return (
        <main className="flex min-h-screen flex-col items-center justify-center p-24">
            <h1 className="text-4xl font-bold mb-8">Game: {roomName}</h1>
            <p className="text-lg text-gray-300 mb-4">Playing as: {username}</p>
            <Chessboard />
        </main>
    );
}
