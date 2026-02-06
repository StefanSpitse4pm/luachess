"use client";
import useWebsocket from '../service/socket'
import { useEffect } from 'react';
import { useRouter } from 'next/navigation';

export default function Games() {
    const router = useRouter();
    const { sendMessage, lastMessage, isConnected } = useWebsocket("ws://localhost:9002")

    useEffect(() => {
        if (isConnected) {
            sendMessage({ type: "Room", payload: {"action":"ListRooms"} });
        }
    }, [isConnected]);

    useEffect(() => {
        console.log("Last message:", lastMessage);
    }, [lastMessage]);

    function handleSubmit(event: React.FormEvent<HTMLFormElement>) {
        event.preventDefault();
        const formData = new FormData(event.currentTarget);
        const roomName = formData.get("roomName") as string;
        const username = formData.get("username") as string;

        sendMessage({ type: "Room", payload: { "action":"CreateRoom" ,"roomName": roomName, "username" : username} });

        // Navigate to room page
        router.push(`/games/room?roomName=${encodeURIComponent(roomName)}&username=${encodeURIComponent(username)}`);
    }

    function handleJoinRoom(roomName: string) {
        const username = prompt("Enter your username:");
        if (username) {
            // Navigate to room page
            router.push(`/games/room?roomName=${encodeURIComponent(roomName)}&username=${encodeURIComponent(username)}`);
        }
    }

    return (
        <main className="flex min-h-screen flex-col items-center justify-between p-24">
            <h1 className="text-4xl font-bold">Games Lobby</h1>

            <div className="w-full max-w-4xl mt-8">
                <div className="bg-gray-800 shadow-lg rounded-lg p-6 mb-6 border border-gray-700">
                    <h2 className="text-2xl font-bold mb-4 text-white">Available Rooms</h2>
                    <ul className="space-y-2">
                        {lastMessage?.rooms?.map((room: any, index: number) => (
                            <li
                                key={index}
                                className="border-2 border-gray-600 py-3 px-4 flex justify-between cursor-pointer hover:bg-gray-700 hover:border-blue-500 rounded transition-all"
                                onClick={() => handleJoinRoom(room.roomName)}
                            >
                                <div className="font-semibold text-lg text-white">{room.roomName}</div>
                                <div className="text-gray-300">
                                    <span className={room.filledSpots === room.roomSize ? 'text-red-400 font-bold' : 'text-green-400'}>
                                        {room.filledSpots} / {room.roomSize}
                                    </span>
                                </div>
                            </li>
                        ))}
                    </ul>
                    {(!lastMessage?.rooms || lastMessage.rooms.length === 0) && (
                        <p className="text-gray-400 text-center py-4">No rooms available. Create one below!</p>
                    )}
                </div>

                <div className="bg-gray-800 shadow-lg rounded-lg p-6 border border-gray-700">
                    <h2 className="text-2xl font-bold mb-4 text-white">Create New Room</h2>
                    <form onSubmit={handleSubmit} className="space-y-4">
                        <div>
                            <label className="block text-sm font-medium mb-1 text-gray-200">Username</label>
                            <input
                                type="text"
                                name="username"
                                placeholder="Enter your username"
                                required
                                className="w-full px-4 py-2 bg-gray-700 border border-gray-600 rounded focus:outline-none focus:ring-2 focus:ring-blue-500 text-white placeholder-gray-400"
                            />
                        </div>
                        <div>
                            <label className="block text-sm font-medium mb-1 text-gray-200">Room Name</label>
                            <input
                                type="text"
                                name="roomName"
                                placeholder="Enter room name"
                                required
                                className="w-full px-4 py-2 bg-gray-700 border border-gray-600 rounded focus:outline-none focus:ring-2 focus:ring-blue-500 text-white placeholder-gray-400"
                            />
                        </div>
                        <button
                            type="submit"
                            className="w-full px-4 py-2 bg-blue-600 text-white rounded hover:bg-blue-700 font-semibold transition-all"
                        >
                            Create Room
                        </button>
                    </form>
                </div>
            </div>
        </main>
    );
}
