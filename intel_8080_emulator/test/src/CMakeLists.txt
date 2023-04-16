include_directories ("../../include")

add_executable(test_instructions test_instructions.c)
add_dependencies(test_instructions test_instructions)
add_test(test_instructions test_instructions)
target_link_libraries(test_instructions instructions i8080 cmocka)

add_executable(test_utils test_utils.c)
add_dependencies(test_utils test_utils)
add_test(test_utils test_utils)
target_link_libraries(test_utils utils i8080 cmocka)
