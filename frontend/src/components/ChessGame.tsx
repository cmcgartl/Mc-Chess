import {useState, useCallback, useEffect} from "react"
//import { Chess } from "chess.js"
import { Chessboard } from "react-chessboard"
import { type GameState } from "../services/api"
import {startGame, makeMove, resetGame} from "../services/api"

const containerStyle: React.CSSProperties = {
    display: 'flex',
    gap: '30px',
    justifyContent: 'center',
    alignItems: 'flex-start',
    padding: '40px',
    minHeight: '100vh',
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

const ChessGame = () => {
    const [game, setGame] = useState<GameState>();

    useEffect(() => {
        startGame().then(state => setGame(state))
    }, [])

    const [moveLog, setMoveLog] = useState<string[]>([])

    const getGameStatus = () => {
        return game?.status
    }

    const reset = () => {
        resetGame()
            .then(state => {
                setGame(state)
                setMoveLog([])
            })
    }

    const onDrop = useCallback(({ sourceSquare, targetSquare }: { piece: unknown; sourceSquare: string; targetSquare: string | null }) => {
        if (!targetSquare) return false
        try{
            makeMove(sourceSquare, targetSquare)
                .then(state => {
                    setGame(state);
                    const moveNotation = `${game?.turn === 'w' ? 'White' : 'Black'}: ${sourceSquare}${targetSquare}`
                    setMoveLog(prev => [...prev, moveNotation])
                }).catch(() => {})
        } catch(error){
            return false
        }

        return false
    }, [game])

    return (
        <div style={containerStyle}>
            <div style={boardContainerStyle}>
                <div style={statusStyle}>
                    {getGameStatus()}
                </div>
                <Chessboard options={{
                    position: game?.FEN,
                    onPieceDrop: onDrop,
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

                    <button
                        onClick={reset}
                        style={buttonStyle}
                        onMouseOver={e => (e.target as HTMLButtonElement).style.backgroundColor = '#1976d2'}
                        onMouseOut={e => (e.target as HTMLButtonElement).style.backgroundColor = '#2196f3'}
                        >
                            New Game
                        </button>

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
