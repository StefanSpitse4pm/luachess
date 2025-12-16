"use client";
import useWebsocket from '../service/socket'
import { useEffect } from 'react';
export default function Games() {

    const { sendMessage, lastMessage, isConnected } = useWebsocket("ws://localhost:9002")

    useEffect(() => {
        if (isConnected) {
            sendMessage({ type: "listRooms", payload: {} });
            console.log("Last message:", lastMessage);
        }
    }, [isConnected]);

    function handleSubmit(event: React.FormEvent<HTMLFormElement>) {
        event.preventDefault();
        const formData = new FormData(event.currentTarget);
        const roomName = formData.get("roomName") as string;
        const username = formData.get("username") as string;
        sendMessage({ type: "createRoom", payload: { "roomName": roomName, "username" : username} });
        sendMessage({ type: "listRooms", payload: {} });
    }


    return (
        <main className="flex min-h-screen flex-col items-center justify-between p-24">
            <h1 className="text-4xl font-bold">Games Page</h1>
            <div>

                <h2 className="text-2xl font-bold">Available Rooms</h2>
                <ul>
                    {lastMessage?.rooms.map((room: any, index: number) => (
                        <li key={index} className="border-b py-2 flex justify-between">
                            <div>{room.roomName}</div>
                            <div>{room.filledSpots} / {room.roomSize}</div>
                        </li>
                    ))}
                </ul>
                <form onSubmit={handleSubmit}>
                    <input type="text" name="username" placeholder="username" required />
                    <input type="text" name="roomName" placeholder="Enter room name" required />
                    <button type="submit" className="mt-2 px-4 py-2 bg-blue-600 text-white rounded hover:bg-blue-700">
                        Create Room
                    </button>
                </form>
            </div>
        </main>
    );
}