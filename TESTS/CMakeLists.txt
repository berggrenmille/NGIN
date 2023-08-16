enable_testing()

# Define test sources
file(GLOB_RECURSE NGIN_TEST_SOURCES "*.cpp")

add_executable(NGIN_TESTS ${NGIN_TEST_SOURCES})

target_compile_definitions(NGIN PRIVATE -DNGIN_TESTING)
target_link_libraries(NGIN_TESTS PRIVATE gtest_main NGIN)


# Register tests
add_test(NAME NGIN_TESTS COMMAND NGIN_TESTS)

#copy NGIN dll to binary dir
add_custom_command(TARGET NGIN_TESTS POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   $<TARGET_FILE:NGIN>
                   $<TARGET_FILE_DIR:NGIN_TESTS>)

#If debug run test automatically, useful for in editor view
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET NGIN_TESTS
                       POST_BUILD
                       COMMAND $<TARGET_FILE:NGIN_TESTS>)
endif()