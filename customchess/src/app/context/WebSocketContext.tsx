"use client";
import React, { createContext, useContext, useEffect, useRef, useState, ReactNode } from 'react';

interface WebSocketContextType {
    sendMessage: (data: any) => void;
    lastMessage: any | null;
    isConnected: boolean;
    publicPlayerId: string;
}

const WebSocketContext = createContext<WebSocketContextType | undefined>(undefined);

export function WebSocketProvider({ children, url }: { children: ReactNode; url: string }) {
    const ws = useRef<WebSocket | null>(null);
    const [lastMessage, setLastMessage] = useState<any | null>(null);
    const [isConnected, setIsConnected] = useState(false);
    const [publicPlayerId, setPublicPlayerId] = useState<string>("");

    useEffect(() => {
        // Create WebSocket connection
        ws.current = new WebSocket(url);

        ws.current.onopen = () => {
            console.log("Connected to WebSocket");
            setIsConnected(true);
        };

        ws.current.onclose = () => {
            console.log("Disconnected from WebSocket");
            setIsConnected(false);
        };

        ws.current.onerror = (error) => {
            console.error("WebSocket error:", error);
            setIsConnected(false);
        };

        ws.current.onmessage = (event) => {
            try {
                console.log("Received message:", event.data);
                const data = JSON.parse(event.data);

                // This can come in a different message than game id / room state.
                // Persist it so pages navigated to later can still access it.
                if (typeof data?.publicPlayerId === "string" && data.publicPlayerId.length > 0) {
                    setPublicPlayerId(data.publicPlayerId);
                }
                setLastMessage(data);
            } catch (err) {
                console.error("Error parsing incoming message:", err);
            }
        };

        // Cleanup on unmount
        return () => {
            if (ws.current) {
                ws.current.close();
                setIsConnected(false);
            }
        };
    }, [url]);

    const sendMessage = (data: any) => {
        if (ws.current && ws.current.readyState === WebSocket.OPEN) {
            ws.current.send(JSON.stringify(data));
        } else {
            console.error("WebSocket is not open");
        }
    };

    return (
        <WebSocketContext.Provider value={{ sendMessage, lastMessage, isConnected, publicPlayerId }}>
            {children}
        </WebSocketContext.Provider>
    );
}

export function useWebSocketContext() {
    const context = useContext(WebSocketContext);
    if (context === undefined) {
        throw new Error('useWebSocketContext must be used within a WebSocketProvider');
    }
    return context;
}
