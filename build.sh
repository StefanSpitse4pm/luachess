#!/bin/bash
set -e

# Always run relative to repo root
cd "$(dirname "$0")"

CXX=g++
CXXFLAGS="-std=c++17"
INCLUDES="-Ibackend/extern/sol2/include -I/usr/include/lua5.4"
LIBS="-llua5.4"

SRC="backend/websocket.cpp backend/chessboard.cpp backend/luaController.cpp"
OUT="backend/websocket"

echo "Compiling..."
$CXX $CXXFLAGS $SRC $INCLUDES $LIBS -o $OUT
echo "Build complete: $OUT"

echo "Running..."
./$OUT