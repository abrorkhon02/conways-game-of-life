cmake_minimum_required(VERSION 3.10)
project(game_of_life)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    add_executable(game_of_life main.cpp CLI.cpp Grid.cpp)
    target_link_libraries(game_of_life PUBLIC OpenMP::OpenMP_CXX)
endif()