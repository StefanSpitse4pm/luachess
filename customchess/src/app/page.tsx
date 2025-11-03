import Image from "next/image";
import Chessboard from "./components/chessboard";
import Link from "next/link";
export default function Home() {
  return (
    <main className="flex min-h-screen flex-col items-center justify-between p-24">
      <h1 className="text-4xl font-bold">Welcome to Custom Chess!</h1>
      <Chessboard />
      <p className="mt-4 text-lg">Your personalized chess experience awaits.</p>
      <Link href="/games" className="mt-6 px-4 py-2 bg-blue-600 text-white rounded hover:bg-blue-700">
        Go to Games
      </Link>
    </main>
  );
}
