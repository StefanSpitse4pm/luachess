"use client";
import { useEffect, useState } from 'react';
import { useRouter, useSearchParams } from 'next/navigation';
import { useWebSocketContext } from '../../context/WebSocketContext';

interface RoomData {
    filledSpots: number;
    players: string[];
    roomName: string;
    roomSize: number;
}

export default function Room() {
    const router = useRouter();
    const searchParams = useSearchParams();
    const roomName = searchParams.get('roomName');
    const username = searchParams.get('username');

    const { sendMessage, lastMessage, isConnected } = useWebSocketContext();
    const [currentRoom, setCurrentRoom] = useState<RoomData | null>(null);

    useEffect(() => {
        if (!roomName || !username) {
            router.push('/games');
            return;
        }
    }, [isConnected, roomName, username]);

    useEffect(() => {
        console.log("Last message:", lastMessage);
        if (lastMessage && lastMessage.roomName && lastMessage.players) {
            setCurrentRoom(lastMessage as RoomData);
        }
    }, [lastMessage]);

    function handleLeaveRoom() {
        if (currentRoom && username) {
            sendMessage({
                type: "Room",
                payload: {
                    "action": "LeaveRoom",
                    "roomName": currentRoom.roomName,
                    "username": username
                }
            });
            router.push('/games');
        }
    }

    function handleStartGame() {
        if (currentRoom && username) {
            sendMessage({
                type: "Game",
                payload: {
                    gameType: "PlayerCreatedLuaGame",
                    roomName: currentRoom.roomName,
                    action: "startGame"
                }
            });
            router.push(`/games/room/play?roomName=${encodeURIComponent(currentRoom.roomName)}&username=${encodeURIComponent(username)}`);
        }
    }

    if (!currentRoom) {
        return (
            <main className="flex min-h-screen flex-col items-center justify-center p-24">
                <div className="text-2xl text-white">Loading room...</div>
            </main>
        );
    }

    return (
        <main className="flex min-h-screen flex-col items-center justify-between p-24">
            <h1 className="text-4xl font-bold">Room: {currentRoom.roomName}</h1>

            <div className="w-full max-w-4xl mt-8">
                <div className="bg-gray-800 shadow-lg rounded-lg p-6 border border-gray-700">
                    <div className="flex justify-between items-center mb-6">
                        <h2 className="text-3xl font-bold text-white">{currentRoom.roomName}</h2>
                        <button
                            onClick={handleLeaveRoom}
                            className="px-4 py-2 bg-red-600 text-white rounded hover:bg-red-700"
                        >
                            Leave Room
                        </button>
                    </div>

                    <div className="mb-6">
                        <div className="text-lg mb-2 text-gray-200">
                            <span className="font-semibold">Room Status:</span> {currentRoom.filledSpots} / {currentRoom.roomSize} players
                        </div>
                        <div className="w-full bg-gray-700 rounded-full h-4">
                            <div
                                className="bg-blue-600 h-4 rounded-full transition-all duration-300"
                                style={{width: `${(currentRoom.filledSpots / currentRoom.roomSize) * 100}%`}}
                            ></div>
                        </div>
                    </div>

                    <div className="mb-6">
                        <h3 className="text-2xl font-semibold mb-4 text-white">Players</h3>
                        <ul className="space-y-2">
                            {currentRoom.players.map((player: string, index: number) => (
                                <li
                                    key={index}
                                    className={`p-3 rounded ${player === username ? 'bg-blue-900 border-2 border-blue-500' : 'bg-gray-700'}`}
                                >
                                    <div className="flex items-center">
                                        <span className="text-lg text-white">{player}</span>
                                        {player === username && (
                                            <span className="ml-2 text-sm text-blue-400 font-semibold">(You)</span>
                                        )}
                                    </div>
                                </li>
                            ))}
                        </ul>
                    </div>

                    {currentRoom.filledSpots === currentRoom.roomSize && (
                        <div className="bg-green-900 border-2 border-green-500 rounded p-4 text-center">
                            <p className="text-green-300 font-semibold text-lg">Room is full! Ready to start game.</p>
                            <button
                                onClick={handleStartGame}
                                className="mt-4 px-6 py-3 bg-green-600 text-white text-lg font-bold rounded hover:bg-green-700 transition-colors"
                            >
                                Start Game
                            </button>
                        </div>
                    )}

                    {currentRoom.filledSpots < currentRoom.roomSize && (
                        <div className="bg-yellow-900 border-2 border-yellow-500 rounded p-4 text-center">
                            <p className="text-yellow-300 font-semibold">Waiting for more players...</p>
                        </div>
                    )}
                </div>
            </div>
        </main>
    );
}
