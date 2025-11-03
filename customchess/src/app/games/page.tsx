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

    useEffect(() => {
        if (lastMessage) {
            console.log("Received message:", lastMessage);
        }
    }, [lastMessage]);

    function handleCreateRoom() {
        sendMessage({ type: "createRoom", payload: { roomName: "New Room",  } });
        sendMessage({ type: "listRooms", payload: {} });
    }

    return (
        <main className="flex min-h-screen flex-col items-center justify-between p-24">
            <h1 className="text-4xl font-bold">Games Page</h1>
            <div>

                <div onClick={handleCreateRoom} className="mt-6 px-4 py-2 bg-blue-600 text-white rounded hover:bg-blue-700">
                    add test room
                </div>
            </div>
        </main>
    );
}