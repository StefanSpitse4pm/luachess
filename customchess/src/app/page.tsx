import Image from "next/image";
import Chessboard from "./components/chessboard";
export default function Home() {
  return (
    <main className="flex min-h-screen flex-col items-center justify-between p-24">
      <h1 className="text-4xl font-bold">Welcome to Custom Chess!</h1>
      <Chessboard />
      <p className="mt-4 text-lg">Your personalized chess experience awaits.</p>
    </main>
  );
}
