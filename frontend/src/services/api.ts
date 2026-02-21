const BASE_URL = "http://localhost:18080";

export interface GameState {
  FEN: string;
  turn: "w" | "b";
  legalMoves: Record<string, string[]>;
  status: any;
}

export async function startGame(): Promise<GameState> {
  const res = await fetch(`${BASE_URL}/startGame`);
  if (!res.ok) throw new Error("Failed to start game");
  return res.json();
}

export async function makeMove(from: string, to: string): Promise<GameState> {
  
  const res = await fetch(`${BASE_URL}/makeMove`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ from, to }),
  });
  if (!res.ok) throw new Error("Invalid move");
  const data = await res.json();
  console.log(data);
  return data;
}

export async function resetGame(): Promise<GameState> {
  const res = await fetch(`${BASE_URL}/reset`, {
    method: "POST",
  });
  if (!res.ok) throw new Error("Invalid move");
  return res.json();
}