module.exports = {

"[externals]/next/dist/compiled/next-server/app-page-turbo.runtime.dev.js [external] (next/dist/compiled/next-server/app-page-turbo.runtime.dev.js, cjs)": ((__turbopack_context__) => {

var { m: module, e: exports } = __turbopack_context__;
{
const mod = __turbopack_context__.x("next/dist/compiled/next-server/app-page-turbo.runtime.dev.js", () => require("next/dist/compiled/next-server/app-page-turbo.runtime.dev.js"));

module.exports = mod;
}}),
"[project]/src/app/components/piece.tsx [app-ssr] (ecmascript)": ((__turbopack_context__) => {
"use strict";

__turbopack_context__.s({
    "default": ()=>Piece
});
var __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2d$jsx$2d$dev$2d$runtime$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__ = __turbopack_context__.i("[project]/node_modules/next/dist/server/route-modules/app-page/vendored/ssr/react-jsx-dev-runtime.js [app-ssr] (ecmascript)");
"use client";
;
function Piece({ position, image, posibleMoves, chessboard }) {
    return /*#__PURE__*/ (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2d$jsx$2d$dev$2d$runtime$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["jsxDEV"])("div", {
        className: `piece w-24 h-24 relative z-10 `,
        style: {
            backgroundImage: `url(${image})`,
            backgroundRepeat: 'no-repeat',
            backgroundPosition: `50% 50%`,
            backgroundSize: '100% 100%'
        }
    }, void 0, false, {
        fileName: "[project]/src/app/components/piece.tsx",
        lineNumber: 10,
        columnNumber: 9
    }, this);
}
}),
"[project]/src/app/service/socket.tsx [app-ssr] (ecmascript)": ((__turbopack_context__) => {
"use strict";

__turbopack_context__.s({
    "default": ()=>__TURBOPACK__default__export__
});
var __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__ = __turbopack_context__.i("[project]/node_modules/next/dist/server/route-modules/app-page/vendored/ssr/react.js [app-ssr] (ecmascript)");
;
function useWebSocket(url) {
    const ws = (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["useRef"])(null);
    const [lastMessage, setLastMessage] = (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["useState"])(null);
    (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["useEffect"])(()=>{
        ws.current = new WebSocket(url);
        ws.current.onopen = ()=>{
            console.log("Connected to WebSocket");
        };
        ws.current.onclose = ()=>{
            console.log("Disconnected from WebSocket");
        };
        ws.current.onerror = (error)=>{
            console.error("WebSocket error:", error);
        };
        ws.current.onmessage = (event)=>{
            try {
                // Parse JSON if your server sends JSON
                const data = JSON.parse(event.data);
                setLastMessage(data);
            } catch (err) {
                console.error("Error parsing incoming message:", err);
            }
        };
        return ()=>{
            ws.current?.close();
        };
    }, [
        url
    ]);
    const sendMessage = (data)=>{
        if (ws.current && ws.current.readyState === WebSocket.OPEN) {
            ws.current.send(JSON.stringify(data));
        } else {
            console.error("Websocket is not open");
        }
    };
    return {
        sendMessage,
        lastMessage
    };
}
const __TURBOPACK__default__export__ = useWebSocket;
}),
"[project]/src/app/components/chessboard.tsx [app-ssr] (ecmascript)": ((__turbopack_context__) => {
"use strict";

__turbopack_context__.s({
    "default": ()=>Chessboard
});
var __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2d$jsx$2d$dev$2d$runtime$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__ = __turbopack_context__.i("[project]/node_modules/next/dist/server/route-modules/app-page/vendored/ssr/react-jsx-dev-runtime.js [app-ssr] (ecmascript)");
var __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__ = __turbopack_context__.i("[project]/node_modules/next/dist/server/route-modules/app-page/vendored/ssr/react.js [app-ssr] (ecmascript)");
var __TURBOPACK__imported__module__$5b$project$5d2f$src$2f$app$2f$components$2f$piece$2e$tsx__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__ = __turbopack_context__.i("[project]/src/app/components/piece.tsx [app-ssr] (ecmascript)");
var __TURBOPACK__imported__module__$5b$project$5d2f$src$2f$app$2f$service$2f$socket$2e$tsx__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__ = __turbopack_context__.i("[project]/src/app/service/socket.tsx [app-ssr] (ecmascript)");
"use client";
;
;
;
;
function Chessboard() {
    const { sendMessage, lastMessage } = (0, __TURBOPACK__imported__module__$5b$project$5d2f$src$2f$app$2f$service$2f$socket$2e$tsx__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["default"])("ws://localhost:9002");
    const [posibleMoves, setPosibleMoves] = (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["useState"])(new Set());
    const [isPieceSelected, setIsPieceSelected] = (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["useState"])(true);
    const [selectedPiece, setSelectedPiece] = (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["useState"])(null);
    const size = 8;
    const [chessboard, setChessboard] = (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["useState"])(()=>{
        const initialBoard = Array.from({
            length: size
        }, ()=>Array(size).fill(null));
        initialBoard[1][3] = {
            position: {
                row: 1,
                col: 3
            },
            image: '/Chess_plt45.svg',
            type: 'pawn',
            posibleMoves: [
                {
                    dx: 1,
                    dy: 1,
                    repeat: true,
                    basedOnlastMove: false
                }
            ],
            posibleTakes: [
                {
                    dx: 5,
                    dy: 3
                }
            ],
            canJump: false
        };
        initialBoard[3][5] = {
            position: {
                row: 3,
                col: 5
            },
            image: '/Chess_pdt45.svg',
            type: 'pawn',
            posibleMoves: [
                {
                    dx: 0,
                    dy: 1,
                    repeat: false,
                    basedOnlastMove: false
                },
                {
                    dx: 1,
                    dy: 1,
                    repeat: true,
                    basedOnlastMove: true
                }
            ]
        };
        return initialBoard;
    });
    (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["useEffect"])(()=>{
        if (lastMessage) {
            console.log(lastMessage);
        }
    }, [
        lastMessage
    ]);
    function handleSquareClick(piecePosition) {
        const newPosibleMoves = new Set();
        isPieceSelected ? setIsPieceSelected(false) : setIsPieceSelected(true);
        if (piecePosition && isPieceSelected) {
            setSelectedPiece(piecePosition);
            let lastdx = 0;
            let lastdy = 0;
            piecePosition.posibleMoves.forEach((move)=>{
                const r = piecePosition.position.row + move.dy + (move.basedOnlastMove ? lastdy : 0);
                const c = piecePosition.position.col + move.dx + (move.basedOnlastMove ? lastdx : 0);
                if (r < 0 || r >= size || c < 0 || c >= size) return;
                newPosibleMoves.add(`${r},${c}`);
                if (move.repeat) {
                    let repeatRow = r;
                    let repeatCol = c;
                    while(repeatRow >= 0 && repeatRow < size && repeatCol >= 0 && repeatCol < size){
                        if (chessboard[repeatRow][repeatCol] !== null && !piecePosition.canJumpOverPieces) break;
                        newPosibleMoves.add(`${repeatRow},${repeatCol}`);
                        repeatRow += move.dy;
                        repeatCol += move.dx;
                    }
                }
                lastdx = move.dx;
                lastdy = move.dy;
            });
            piecePosition.posibleTakes?.forEach((take)=>{
                console.log(take);
                if (take.dx < 0 || take.dy < 0 || take.dx >= size || take.dy >= size) return;
                newPosibleMoves.add(`${take.dy},${take.dx}`);
                console.log(newPosibleMoves);
            });
            setPosibleMoves(newPosibleMoves);
        }
        return;
    }
    function movePiece(moveTo) {
        console.log(`Moving piece to: ${moveTo.row}, ${moveTo.col}`);
        if (selectedPiece) {
            const { row, col } = selectedPiece.position;
            if (chessboard[moveTo.row][moveTo.col] !== null && selectedPiece.posibleTakes) {
                chessboard[moveTo.row][moveTo.col] = null;
                selectedPiece.posibleTakes = selectedPiece.posibleTakes.filter((take)=>!(moveTo.row === take.dy) && moveTo.col === take.dx);
            }
            setChessboard((prevBoard)=>{
                const newBoard = prevBoard.map((row)=>[
                        ...row
                    ]);
                newBoard[moveTo.row][moveTo.col] = {
                    ...selectedPiece,
                    position: moveTo
                };
                newBoard[row][col] = null;
                return newBoard;
            });
            setSelectedPiece(null);
            setPosibleMoves(new Set());
            setIsPieceSelected(true);
            sendMessage({
                type: "ChessboardState",
                payload: {
                    "test": "test"
                }
            });
        }
    }
    return /*#__PURE__*/ (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2d$jsx$2d$dev$2d$runtime$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["jsxDEV"])("div", {
        className: "chessboard",
        children: Array.from({
            length: size
        }).map((_, row)=>/*#__PURE__*/ (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2d$jsx$2d$dev$2d$runtime$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["jsxDEV"])("div", {
                className: "flex",
                children: Array.from({
                    length: size
                }).map((_, col)=>{
                    const isWhite = (row + col) % 2 === 0;
                    const posKey = `${row},${col}`;
                    const isHighlighted = posibleMoves.has(posKey) && !isPieceSelected;
                    return /*#__PURE__*/ (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2d$jsx$2d$dev$2d$runtime$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["jsxDEV"])("div", {
                        className: `chessboard-square-${row}-${col} ${isWhite ? 'bg-white' : 'bg-purple-900'} w-24 h-24 flex relative items-center justify-center`,
                        onClick: ()=>handleSquareClick(chessboard[row][col]),
                        children: [
                            chessboard[row][col] ? /*#__PURE__*/ (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2d$jsx$2d$dev$2d$runtime$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["jsxDEV"])(__TURBOPACK__imported__module__$5b$project$5d2f$src$2f$app$2f$components$2f$piece$2e$tsx__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["default"], {
                                image: chessboard[row][col].image,
                                position: chessboard[row][col].position,
                                type: chessboard[row][col].type,
                                posibleMoves: chessboard[row][col].posibleMoves,
                                chessboard: chessboard
                            }, void 0, false, {
                                fileName: "[project]/src/app/components/chessboard.tsx",
                                lineNumber: 108,
                                columnNumber: 37
                            }, this) : /*#__PURE__*/ (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2d$jsx$2d$dev$2d$runtime$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["jsxDEV"])("div", {
                                className: `w-8 h-8 rounded-full bg-gray-400 opacity-40 ${isHighlighted ? 'block' : 'hidden'}`,
                                onClick: ()=>movePiece({
                                        row,
                                        col
                                    })
                            }, void 0, false, {
                                fileName: "[project]/src/app/components/chessboard.tsx",
                                lineNumber: 110,
                                columnNumber: 37
                            }, this),
                            chessboard[row][col] && posibleMoves.has(posKey) && /*#__PURE__*/ (0, __TURBOPACK__imported__module__$5b$project$5d2f$node_modules$2f$next$2f$dist$2f$server$2f$route$2d$modules$2f$app$2d$page$2f$vendored$2f$ssr$2f$react$2d$jsx$2d$dev$2d$runtime$2e$js__$5b$app$2d$ssr$5d$__$28$ecmascript$29$__["jsxDEV"])("div", {
                                className: `w-24 h-24 rounded-full bg-transparent border-gray-500 border-7 opacity-40 z-20 absolute ${isHighlighted ? 'block' : 'hidden'}`,
                                onClick: ()=>movePiece({
                                        row,
                                        col
                                    })
                            }, void 0, false, {
                                fileName: "[project]/src/app/components/chessboard.tsx",
                                lineNumber: 113,
                                columnNumber: 37
                            }, this)
                        ]
                    }, col, true, {
                        fileName: "[project]/src/app/components/chessboard.tsx",
                        lineNumber: 102,
                        columnNumber: 29
                    }, this);
                })
            }, row, false, {
                fileName: "[project]/src/app/components/chessboard.tsx",
                lineNumber: 96,
                columnNumber: 17
            }, this))
    }, void 0, false, {
        fileName: "[project]/src/app/components/chessboard.tsx",
        lineNumber: 94,
        columnNumber: 9
    }, this);
}
}),
"[project]/node_modules/next/dist/server/route-modules/app-page/module.compiled.js [app-ssr] (ecmascript)": ((__turbopack_context__) => {
"use strict";

var { m: module, e: exports } = __turbopack_context__;
{
if ("TURBOPACK compile-time falsy", 0) //TURBOPACK unreachable
;
else {
    if ("TURBOPACK compile-time falsy", 0) //TURBOPACK unreachable
    ;
    else {
        if ("TURBOPACK compile-time truthy", 1) {
            if ("TURBOPACK compile-time truthy", 1) {
                module.exports = __turbopack_context__.r("[externals]/next/dist/compiled/next-server/app-page-turbo.runtime.dev.js [external] (next/dist/compiled/next-server/app-page-turbo.runtime.dev.js, cjs)");
            } else //TURBOPACK unreachable
            ;
        } else //TURBOPACK unreachable
        ;
    }
} //# sourceMappingURL=module.compiled.js.map
}}),
"[project]/node_modules/next/dist/server/route-modules/app-page/vendored/ssr/react-jsx-dev-runtime.js [app-ssr] (ecmascript)": ((__turbopack_context__) => {
"use strict";

var { m: module, e: exports } = __turbopack_context__;
{
module.exports = __turbopack_context__.r("[project]/node_modules/next/dist/server/route-modules/app-page/module.compiled.js [app-ssr] (ecmascript)").vendored['react-ssr'].ReactJsxDevRuntime; //# sourceMappingURL=react-jsx-dev-runtime.js.map
}}),
"[project]/node_modules/next/dist/server/route-modules/app-page/vendored/ssr/react.js [app-ssr] (ecmascript)": ((__turbopack_context__) => {
"use strict";

var { m: module, e: exports } = __turbopack_context__;
{
module.exports = __turbopack_context__.r("[project]/node_modules/next/dist/server/route-modules/app-page/module.compiled.js [app-ssr] (ecmascript)").vendored['react-ssr'].React; //# sourceMappingURL=react.js.map
}}),

};

//# sourceMappingURL=%5Broot-of-the-server%5D__0a679080._.js.map