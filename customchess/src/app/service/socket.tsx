import { useEffect, useRef, useState } from "react";
import { piece } from '../types/piece';

function useWebSocket<T = any>(url: string) {
  const ws = useRef<null | WebSocket>(null);
  const [lastMessage, setLastMessage] = useState<piece[] | null>(null);
  const [isConnected, setIsConnected] = useState(false);

  useEffect(() => {
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
        console.log(event.data);
        const data = JSON.parse(event.data) as piece[];
        setLastMessage(data);
      } catch (err) {
        console.error("Error parsing incoming message:", err);
      }
    };

    return () => {
      ws.current?.close();
      setIsConnected(false);
    };
  }, [url]);

  const sendMessage = (data: any) => {
    if (ws.current && ws.current.readyState === WebSocket.OPEN) {
        ws.current.send(JSON.stringify(data));
    } else {
        console.error("Websocket is not open");
    }

  }

  return { sendMessage, lastMessage, isConnected };
}
export default useWebSocket;