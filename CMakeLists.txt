cmake_minimum_required(VERSION 3.16)
project(luachess
        VERSION 0.1
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(luachess
        src/websocket.cpp
        src/Chess/chessboard.cpp
        src/Handlers/Handler.h
        src/Handlers/Rooms/RoomHandler.cpp
        src/Handlers/Rooms/RoomHandler.h
        src/Handlers/Rooms/Room.cpp
        src/Handlers/Rooms/Room.h
        src/Handlers/ActionContext.h
        src/Handlers/Games/GameHandler.cpp
        src/Handlers/Games/GameHandler.h
        src/Chess/Piece.cpp
        src/Chess/Piece.h
        src/Chess/Move.h
        src/Handlers/Rooms/Player.h
        src/Handlers/Games/LuaGame.cpp
        src/Handlers/Games/LuaGame.h
        src/Handlers/Games/Engine/Engine.h
        src/Handlers/Games/Engine/LuaEngine.cpp
        src/Handlers/Games/Engine/LuaEngine.h
        src/Handlers/Games/GameFactory.h
        src/Handlers/Games/PlayerCreatedLuaGameFactory.cpp
        src/Handlers/Games/PlayerCreatedLuaGameFactory.h
        src/Handlers/Handler.cpp
        src/Handlers/Games/Decorator/GameDecorator.h
        src/Handlers/Games/Decorator/GameDecorator.cpp
        src/Handlers/Games/Decorator/TurnOrderDecorator.cpp
        src/Handlers/Games/Decorator/TurnOrderDecorator.h
        src/Handlers/Games/TurnOrder.cpp
        src/Handlers/Games/TurnOrder.h
)

add_subdirectory(${PROJECT_SOURCE_DIR}/extern/sol2)

target_include_directories(luachess
        PRIVATE
        /usr/include/lua5.4
)

target_link_libraries(luachess
        PRIVATE
        sol2
        lua5.4
)

add_executable(tests
        tests/GameHandlerTests.cpp
        tests/GameTests.cpp
        src/Chess/chessboard.cpp
        src/Handlers/Handler.h
        src/Handlers/Rooms/RoomHandler.cpp
        src/Handlers/Rooms/RoomHandler.h
        src/Handlers/Rooms/Room.cpp
        src/Handlers/Rooms/Room.h
        src/Handlers/ActionContext.h
        src/Handlers/Games/GameHandler.cpp
        src/Handlers/Games/GameHandler.h
        src/Chess/Piece.cpp
        src/Chess/Piece.h
        src/Chess/Move.h
        src/Handlers/Rooms/Player.h
        src/Handlers/Games/LuaGame.cpp
        src/Handlers/Games/LuaGame.h
        src/Handlers/Games/Engine/Engine.h
        src/Handlers/Games/Engine/LuaEngine.cpp
        src/Handlers/Games/Engine/LuaEngine.h
        src/Handlers/Games/GameFactory.h
        src/Handlers/Games/PlayerCreatedLuaGameFactory.cpp
        src/Handlers/Games/PlayerCreatedLuaGameFactory.h
        src/Handlers/Handler.cpp
        src/Handlers/Games/TurnOrder.cpp
        src/Handlers/Games/TurnOrder.h
        tests/TurnOrderTests.cpp
)

target_include_directories(tests
        PRIVATE
        /usr/include/lua5.4

)

target_link_libraries(tests
        PRIVATE
        GTest::gtest_main
        sol2
        lua5.4
)

include(GoogleTest)
gtest_discover_tests(tests)

file(COPY ${CMAKE_SOURCE_DIR}/src/lua DESTINATION ${CMAKE_BINARY_DIR})
