cmake_minimum_required(VERSION 3.5)
project(avl_tree)

set(CMAKE_CXX_STANDARD 20)

include(CodeCoverage.cmake)
add_compile_options(-Wall -Wextra -Wpedantic -Werror -fsanitize=address)
add_link_options(-fsanitize=address)
add_executable(${CMAKE_PROJECT_NAME} main.cc)
add_subdirectory(libs)
add_subdirectory(submodules/Catch2)
enable_testing()
add_subdirectory(tests)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Catch2::Catch2WithMain bst)
