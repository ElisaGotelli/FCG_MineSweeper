cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

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

# TAPPA 4b
add_executable(Tappa4b Tappa_4b/4b_interactive_control_panel.cpp)
target_compile_features(Tappa4b PRIVATE cxx_std_17)
target_link_libraries(Tappa4b PRIVATE SFML::Graphics)

# TAPPA 5
add_executable(Tappa5 Tappa_5/5_start_panel.cpp)
target_compile_features(Tappa5 PRIVATE cxx_std_17)
target_link_libraries(Tappa5 PRIVATE SFML::Graphics)

# TAPPA 6
add_executable(Tappa6 Tappa_6/6_levels.cpp)
target_compile_features(Tappa6 PRIVATE cxx_std_17)
target_link_libraries(Tappa6 PRIVATE SFML::Graphics)

# TAPPA 7b
add_executable(Tappa7a Tappa_7a/7a_graphic_exit.cpp)
target_compile_features(Tappa7a PRIVATE cxx_std_17)
target_link_libraries(Tappa7a PRIVATE SFML::Graphics)

# TAPPA 7b
add_executable(Tappa7b Tappa_7b/7b_interactive_exit.cpp)
target_compile_features(Tappa7b PRIVATE cxx_std_17)
target_link_libraries(Tappa7b PRIVATE SFML::Graphics)

# TAPPA 8
add_executable(Tappa8 Tappa_8/8_rules_and_title.cpp)
target_compile_features(Tappa8 PRIVATE cxx_std_17)
target_link_libraries(Tappa8 PRIVATE SFML::Graphics)

