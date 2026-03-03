import {useState, useCallback, useEffect, useRef} from "react"
//import { Chess } from "chess.js"
import { Chessboard } from "react-chessboard"
import { type GameState } from "../services/api"
import {startGame, makeMove, resetGame, setEngine} from "../services/api"

const containerStyle: React.CSSProperties = {
    display: 'flex',
    gap: '30px',
    justifyContent: 'center',
    alignItems: 'flex-start',
    padding: '20px',
    height: '100vh',
    boxSizing: 'border-box',
    backgroundColor: '#1a1a2e',
    color: '#eee',
    fontFamily: "'Segoe UI', Tahoma, Geneva, Verdana, sans-serif",
}

const boardContainerStyle: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center',
    gap: '15px',
}

const statusStyle: React.CSSProperties = {
    fontSize: '20px',
    fontWeight: 'bold',
    padding: '10px 20px',
    backgroundColor: '#16213e',
    borderRadius: '8px',
    textAlign: 'center',
    minWidth: '250px',
}

const buttonStyle: React.CSSProperties = {
    padding: '10px 24px',
    fontSize: '15px',
    fontWeight: '600',
    color: '#fff',
    backgroundColor: '#2196f3',
    border: 'none',
    borderRadius: '6px',
    cursor: 'pointer',
    transition: 'background-color 0.2s',
}

const moveLogStyle: React.CSSProperties = {
    width: '220px',
    maxHeight: '500px',
    backgroundColor: '#16213e',
    borderRadius: '8px',
    padding: '20px',
    overflowY: 'auto',
}

const moveListStyle: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'column',
    gap: '4px',
}

const moveItemStyle: React.CSSProperties = {
    padding: '6px 10px',
    backgroundColor: '#0f3460',
    borderRadius: '4px',
    fontSize: '14px',
}

function movePieceInFEN(fen: string, from: string, to: string): string { 
    const ranks = fen.split(' ')[0].split('/') 
    const grid: string[][] = ranks.map(rank => { 
        const row: string[] = [] 
        for (const ch of rank) { 
            if (ch >= '1' && ch <= '8') { 
                for (let i = 0; i < parseInt(ch); i++) row.push('') 
            } 
        else { 
            row.push(ch) 
        } 
    } 
    return row 
    }) 
    const fromFile = from.charCodeAt(0) - 97 
    const fromRank = 8 - parseInt(from[1]) 
    const toFile = to.charCodeAt(0) - 97 
    const toRank = 8 - parseInt(to[1]) 
    grid[toRank][toFile] = grid[fromRank][fromFile] 
    grid[fromRank][fromFile] = '' 
    const newPlacement = grid.map(row => { 
        let s = '', empty = 0 
        for (const cell of row) { 
            if (cell === '') {
                empty++ 
            } 
            else {
                if (empty > 0) { 
                    s += empty; 
                    empty = 0 
                } 
                s += cell 
            } 
        } 
        if (empty > 0) s += empty 
        return s 
    }).join('/') 
    const rest = fen.split(' ').slice(1).join(' ') 
    return newPlacement + (rest ? ' ' + rest : '') 
}

const ChessGame = () => {
    const [game, setGame] = useState<GameState>();
    const gameRef = useRef<GameState | undefined>(game);
    gameRef.current = game;
    const [highlightStyles, setHighlightStyles] = useState<Record<string, React.CSSProperties>>({})

    useEffect(() => {
        startGame().then(state => setGame(state))
    }, [])

    const [moveLog, setMoveLog] = useState<string[]>([])
    const [engineSide, setEngineSide] = useState<"off" | "white" | "black">("off")
    const [engineDepth, setEngineDepth] = useState(4)
    const [gameStarted, setGameStarted] = useState(false)

    const getGameStatus = () => {
        return game?.status
    }

    const reset = () => {
        resetGame()
            .then(state => {
                setGame(state)
                setMoveLog([])
                setGameStarted(false)
                return setEngine(engineSide, engineDepth)
            })
            .then(state => setGame(state))
    }

    const onDrag = useCallback(({square} : {isSparePiece: boolean; piece: {pieceType : string}; square: string | null }) => {
        const current = gameRef.current
        if(square && current){
            const moves = current.legalMoves[square]
            if(moves){
                const styles: Record<string, React.CSSProperties> = {}
                moves.forEach(move => {
                    styles[move] = { backgroundColor: 'rgba(255, 255, 0, 0.4)' }
                })
                setHighlightStyles(styles)
            }
        }
}, [])

    const onDrop = useCallback(({ sourceSquare, targetSquare }: { piece: unknown; sourceSquare: string; targetSquare: string | null }) => {
        setHighlightStyles({})
        if (!targetSquare) return false

        const moves = game?.legalMoves[sourceSquare]
        if(!moves || !moves.includes(targetSquare) || game.engineTurn) return false

        const optimisticGame = {
            ...game!,
            FEN: movePieceInFEN(game!.FEN, sourceSquare, targetSquare)
        };
        setGame(optimisticGame);
        try{
            makeMove(sourceSquare, targetSquare)
                .then(state => {
                    setGame(state);
                    const playerMove = `${game?.turn === 'w' ? 'White' : 'Black'}: ${sourceSquare}${targetSquare}`
                    setMoveLog(prev => [...prev, playerMove])
                    setGameStarted(true)
                    if(state.engineMoveFrom && state.engineMoveTo){
                        const engineMove = `${game?.turn === 'w' ? 'Black' : 'White'}: ${state.engineMoveFrom}${state.engineMoveTo}`
                        setMoveLog(prev => [...prev, engineMove])
                    }
                }).catch((err) => console.error("makeMove failed:", err))
        } catch(error){
            return false
        }

        return true
    }, [game])

    return (
        <div style={containerStyle}>
            <div style={boardContainerStyle}>
                <div style={statusStyle}>
                    {getGameStatus()}
                </div>
                <div style={{width: '500px'}}>
                <Chessboard options={{
                    position: game?.FEN,
                    onPieceDrop: onDrop,
                    onPieceDrag: onDrag,
                    showAnimations: false,
                    squareStyles: highlightStyles,
                    boardStyle: {
                        borderRadius: '4px',
                        boxShadow: '0 2px 10px rgba(0,0,0,0.3)'
                    },
                    darkSquareStyle: {
                        backgroundColor: "#779952"
                    },
                    lightSquareStyle: {
                        backgroundColor: "#edeed1"
                    },
                }} />
                </div>

                    <button
                        onClick={reset}
                        style={buttonStyle}
                        onMouseOver={e => (e.target as HTMLButtonElement).style.backgroundColor = '#1976d2'}
                        onMouseOut={e => (e.target as HTMLButtonElement).style.backgroundColor = '#2196f3'}
                        >
                            New Game
                        </button>

                    <div style={{display: 'flex', gap: '15px', alignItems: 'center', backgroundColor: '#16213e', padding: '10px 20px', borderRadius: '8px'}}>
                        <label>Engine:</label>
                        <select
                            value={engineSide}
                            disabled={gameStarted}
                            onChange={async (e) => {
                                const side = e.target.value as "off" | "white" | "black"
                                setEngineSide(side)
                                const state = await setEngine(side, engineDepth)
                                setGame(state)
                                if(state.engineMoveFrom && state.engineMoveTo){
                                    const engineMove = `${side === 'white' ? 'White' : 'Black'}: ${state.engineMoveFrom}${state.engineMoveTo}`
                                    setMoveLog(prev => [...prev, engineMove])
                                    setGameStarted(true)
                                }
                            }}
                            style={{padding: '6px 10px', borderRadius: '4px', backgroundColor: '#0f3460', color: '#eee', border: '1px solid #333', cursor: gameStarted ? 'not-allowed' : 'pointer', opacity: gameStarted ? 0.5 : 1}}
                        >
                            <option value="off">Off</option>
                            <option value="black">Black</option>
                            <option value="white">White</option>
                        </select>
                        <label>Depth:</label>
                        <select
                            value={engineDepth}
                            disabled={gameStarted}
                            onChange={async (e) => {
                                const depth = parseInt(e.target.value)
                                setEngineDepth(depth)
                                const state = await setEngine(engineSide, depth)
                                setGame(state)
                            }}
                            style={{padding: '6px 10px', borderRadius: '4px', backgroundColor: '#0f3460', color: '#eee', border: '1px solid #333', cursor: gameStarted ? 'not-allowed' : 'pointer', opacity: gameStarted ? 0.5 : 1}}
                        >
                            {[1, 2, 3, 4, 5, 6].map(d => (
                                <option key={d} value={d}>{d}</option>
                            ))}
                        </select>
                    </div>

            </div>
            <div style={moveLogStyle}>
                <h2 style={{marginBottom: '15px', fontSize: '18px'}}>
                    Move History
                </h2>

                <div style={moveListStyle}>
                    {moveLog.length > 0 ? (
                        moveLog.map((move, index) => (
                            <div key={index} style={moveItemStyle}>
                                {`${Math.floor(index/2) + 1}. ${move}`}
                            </div>
                        ))
                    ) : (
                        <div style={{textAlign: 'center', color: '#666', fontStyle: 'italic'}}>
                            No moves yet
                        </div>
                        )}
                </div>
            </div>
        </div>
    )
}

export default ChessGame;
