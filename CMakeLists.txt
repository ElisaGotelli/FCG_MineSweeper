cmake_minimum_required(VERSION 3.21)

project(FCG_MINESWEEPER LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

###############TAPPE##########################

#TAPPA 00
add_executable(Tappa00  Tappa_00/00_hello.cpp)
target_link_libraries(Tappa00  PRIVATE  SFML::Graphics)

#TAPPA 1a
add_executable(Tappa1a  Tappa_1a/1a_cell_starting_texture.cpp)
target_link_libraries(Tappa1a  PRIVATE  SFML::Graphics)

#TAPPA 1b
add_executable(Tappa1b  Tappa_1b/1b_interactive_cell.cpp)
target_link_libraries(Tappa1b  PRIVATE  SFML::Graphics)

