"use client";
import React, { createContext, useContext, ReactNode } from 'react';

interface WebSocketContextType {
    sendMessage: (data: any) => void;
    lastMessage: any | null;
    isConnected: boolean;
    publicPlayerId: string;
}

const WebSocketContext = createContext<WebSocketContextType | undefined>(undefined);

type WebSocketProviderProps = {
    children: ReactNode;
    url: string;
};

type WebSocketProviderState = {
    lastMessage: any | null;
    isConnected: boolean;
    publicPlayerId: string;
};

export class WebSocketProvider extends React.Component<WebSocketProviderProps, WebSocketProviderState> {
    private ws: WebSocket | null = null;

    state: WebSocketProviderState = {
        lastMessage: null,
        isConnected: false,
        publicPlayerId: "",
    };

    componentDidMount() {
        this.connect(this.props.url);
    }

    componentDidUpdate(prevProps: Readonly<WebSocketProviderProps>) {
        if (prevProps.url !== this.props.url) {
            this.disconnect();
            this.connect(this.props.url);
        }
    }

    componentWillUnmount() {
        this.disconnect();
    }

    private connect(url: string) {
        // Create WebSocket connection
        this.ws = new WebSocket(url);

        this.ws.onopen = () => {
            console.log("Connected to WebSocket");
            this.setState({ isConnected: true });
        };

        this.ws.onclose = () => {
            console.log("Disconnected from WebSocket");
            this.setState({ isConnected: false });
        };

        this.ws.onerror = (error) => {
            console.error("WebSocket error:", error);
            this.setState({ isConnected: false });
        };

        this.ws.onmessage = (event) => {
            try {
                console.log("Received message:", event.data);
                const data = JSON.parse(event.data);

                if (typeof data?.publicPlayerId === "string" && data.publicPlayerId.length > 0) {
                    this.setState({ publicPlayerId: data.publicPlayerId });
                }
                this.setState({ lastMessage: data });
            } catch (err) {
                console.error("Error parsing incoming message:", err);
            }
        };
    }

    private disconnect() {
        if (this.ws) {
            try {
                // Clear handlers to avoid setState after unmount.
                this.ws.onopen = null;
                this.ws.onclose = null;
                this.ws.onerror = null;
                this.ws.onmessage = null;
                this.ws.close();
            } catch {
                // ignore
            }
            this.ws = null;
        }

        // Ensure consumers see disconnected state.
        this.setState({ isConnected: false });
    }

    private sendMessage = (data: any) => {
        if (this.ws && this.ws.readyState === WebSocket.OPEN) {
            this.ws.send(JSON.stringify(data));
        } else {
            console.error("WebSocket is not open");
        }
    };

    render() {
        const { children } = this.props;
        const { lastMessage, isConnected, publicPlayerId } = this.state;

        return (
            <WebSocketContext.Provider
                value={{
                    sendMessage: this.sendMessage,
                    lastMessage,
                    isConnected,
                    publicPlayerId,
                }}
            >
                {children}
            </WebSocketContext.Provider>
        );
    }
}

export function useWebSocketContext() {
    const context = useContext(WebSocketContext);
    if (context === undefined) {
        throw new Error('useWebSocketContext must be used within a WebSocketProvider');
    }
    return context;
}
