enable_testing()

# Define test sources
file(GLOB_RECURSE NGIN_TEST_SOURCES "*.cpp")

add_executable(NGINTests ${NGIN_TEST_SOURCES})
target_link_libraries(NGINTests PRIVATE gtest_main NGIN)

# Register tests
add_test(NAME NGINTests COMMAND NGINTests)

add_custom_command(TARGET NGINTests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   $<TARGET_FILE:NGIN>
                   $<TARGET_FILE_DIR:NGINTests>)

add_custom_command(TARGET NGINTests
                   POST_BUILD
                   COMMAND $<TARGET_FILE:NGINTests>)
