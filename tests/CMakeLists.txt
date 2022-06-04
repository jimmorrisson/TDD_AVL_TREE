add_executable(bst_tests bst_tests.cpp)
target_link_libraries(
    bst_tests
    PRIVATE
    Catch2::Catch2WithMain
)

add_test(NAME BstTest COMMAND bst_tests)
option (GENERATE_CODE_COVERAGE "Generate code coverage" OFF)
if (GENERATE_CODE_COVERAGE)
    APPEND_COVERAGE_COMPILER_FLAGS()
    SETUP_TARGET_FOR_COVERAGE_LCOV(
            NAME coverage                 
            EXECUTABLE ctest -j ${n_cores} # Executable in PROJECT_BINARY_DIR
            DEPENDENCIES
            bst_tests)
endif()