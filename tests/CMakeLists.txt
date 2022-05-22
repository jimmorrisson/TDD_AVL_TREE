add_executable(bst_tests bst_tests.cpp)
target_link_libraries(
    bst_tests
    PRIVATE
    Catch2::Catch2WithMain
    bst
)

add_test(NAME BstTest COMMAND bst_tests)