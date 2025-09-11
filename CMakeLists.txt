cmake_minimum_required(VERSION 3.21)

project(FCG_MINESWEEPER LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

# Metti tutti gli eseguibili dentro build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Copia la cartella risorse accanto agli eseguibili
add_custom_target(copy_risorse ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/risorse
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/risorse
)

############### TAPPE ##########################

# TAPPA 00
add_executable(Tappa00 Tappa_00/00_hello.cpp)
target_link_libraries(Tappa00 PRIVATE SFML::Graphics)
add_dependencies(Tappa00 copy_risorse)

# TAPPA 1a
add_executable(Tappa1a Tappa_1a/1a_cell_starting_texture.cpp)
target_link_libraries(Tappa1a PRIVATE SFML::Graphics)
add_dependencies(Tappa1a copy_risorse)

# TAPPA 1b
add_executable(Tappa1b Tappa_1b/1b_interactive_cell.cpp)
target_link_libraries(Tappa1b PRIVATE SFML::Graphics)
add_dependencies(Tappa1b copy_risorse)

# TAPPA 1c
add_executable(Tappa1c Tappa_1c/1c.cpp)
target_link_libraries(Tappa1c PRIVATE SFML::Graphics)
add_dependencies(Tappa1c copy_risorse)
