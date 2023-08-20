enable_testing()

# Include Dependencies 
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeDependencies.cmake)

# Define test sources
file(GLOB_RECURSE NGIN_TEST_SOURCES "*.cpp")

add_executable(NGIN_TESTS ${NGIN_TEST_SOURCES})

target_compile_definitions(NGIN PRIVATE -DNGIN_TESTING)
target_link_libraries(NGIN_TESTS PRIVATE gtest_main NGIN)


# Register tests
add_test(NAME NGIN_TESTS COMMAND NGIN_TESTS)



# After the NGIN target is built, glob for DLLs and copy them
add_custom_command(TARGET NGIN_TESTS POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -DDST_DIR="$<TARGET_FILE_DIR:NGIN_TESTS>"
    -DSRC_DIR="$<TARGET_FILE_DIR:NGIN>"
    -P ${CMAKE_SOURCE_DIR}/CMakeCopyDll.cmake
)


#If debug run test automatically, useful for in editor view
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET NGIN_TESTS
                       POST_BUILD
                       COMMAND $<TARGET_FILE:NGIN_TESTS>)
endif()