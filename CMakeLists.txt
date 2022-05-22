cmake_minimum_required(VERSION 3.22)
project(avl_tree)

set(CMAKE_CXX_STANDARD 20)

add_executable(${CMAKE_PROJECT_NAME} main.cc)

add_subdirectory(libs)
add_subdirectory(submodules/Catch2)
enable_testing()
add_subdirectory(tests)