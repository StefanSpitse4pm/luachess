cmake_minimum_required(VERSION 3.16)
project(luachess
        VERSION 0.1
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

add_executable(luachess
        src/websocket.cpp
        src/chessboard.cpp
        src/luaController.cpp
        src/Handlers/Handler.h
        src/Handlers/Rooms/RoomHandler.cpp
        src/Handlers/Rooms/RoomHandler.h
        src/Handlers/Rooms/Room.cpp
        src/Handlers/Rooms/Room.h
        src/Handlers/ActionContext.h
)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/extern/sol2)

target_include_directories(luachess
        PRIVATE
        /usr/include/lua5.4
)

target_link_libraries(luachess
        PRIVATE
        sol2
        lua5.4
)


file(COPY ${CMAKE_SOURCE_DIR}/src/lua DESTINATION ${CMAKE_BINARY_DIR})
