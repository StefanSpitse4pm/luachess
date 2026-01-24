cmake_minimum_required(VERSION 3.16)
project(luachess
        VERSION 0.1
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(luachess src/websocket.cpp src/chessboard.cpp src/luaController.cpp)

target_include_directories(luachess PRIVATE ${CMAKE_SOURCE_DIR}/src/extern/sol2/include /usr/include/lua5.4)
target_link_libraries(luachess PRIVATE lua5.4)

file(COPY ${CMAKE_SOURCE_DIR}/src/lua DESTINATION ${CMAKE_BINARY_DIR})