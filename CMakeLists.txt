cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

############### TAPPE ##########################

# TAPPA 00
add_executable(Tappa00 Tappa_00/00_hello.cpp)
target_compile_features(Tappa00 PRIVATE cxx_std_17)
target_link_libraries(Tappa00 PRIVATE SFML::Graphics)

# TAPPA 1a
add_executable(Tappa1a Tappa_1a/1a_cell_starting_texture.cpp)
target_compile_features(Tappa1a PRIVATE cxx_std_17)
target_link_libraries(Tappa1a PRIVATE SFML::Graphics)

# TAPPA 1b
add_executable(Tappa1b Tappa_1b/1b_interactive_cell.cpp)
target_compile_features(Tappa1b PRIVATE cxx_std_17)
target_link_libraries(Tappa1b PRIVATE SFML::Graphics)

# TAPPA 1c
add_executable(Tappa1c Tappa_1c/1c_victory_defeat_restart.cpp)
target_compile_features(Tappa1c PRIVATE cxx_std_17)
target_link_libraries(Tappa1c PRIVATE SFML::Graphics)

# TAPPA 2a
add_executable(Tappa2a Tappa_2a/2a_header.cpp)
target_compile_features(Tappa2a PRIVATE cxx_std_17)
target_link_libraries(Tappa2a PRIVATE SFML::Graphics)

# TAPPA 2b
add_executable(Tappa2b Tappa_2b/2b_intective_header.cpp)
target_compile_features(Tappa2b PRIVATE cxx_std_17)
target_link_libraries(Tappa2b PRIVATE SFML::Graphics)

# TAPPA 3
add_executable(Tappa3 Tappa_3/3_border.cpp)
target_compile_features(Tappa3 PRIVATE cxx_std_17)
target_link_libraries(Tappa3 PRIVATE SFML::Graphics)

# TAPPA 4a
add_executable(Tappa4a Tappa_4a/4a_graphic_control_panel.cpp)
target_compile_features(Tappa4a PRIVATE cxx_std_17)
target_link_libraries(Tappa4a PRIVATE SFML::Graphics)

