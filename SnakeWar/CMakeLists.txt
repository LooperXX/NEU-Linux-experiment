cmake_minimum_required(VERSION 3.12)
project(SnakeWar)

set(CMAKE_CXX_STANDARD 14 )
set(CMAKE_CXX_FLAGS "-lncurses")
set(CMAKE_C_STANDARD "${CMAKE_C_FLAGS} -Wall -Werror -lpdcurses")

add_executable(SnakeWarServer server.cpp)
add_executable(SnakeWarClient client.cpp)
add_executable(SnakeWar game.cpp)
add_executable(TestT testttt.cpp)

find_package(Threads REQUIRED)
target_link_libraries(SnakeWar Threads::Threads)
target_link_libraries(SnakeWarClient Threads::Threads)
target_link_libraries(SnakeWarServer Threads::Threads)
target_link_libraries(TestT Threads::Threads)




