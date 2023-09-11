

# Include Dependencies 
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeDependencies.cmake)

macro(ngin_add_test_macro TARGET_NAME SOURCE_DIR)
    file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/NGIN/NGIN/${SOURCE_DIR}/*.cpp")

    enable_testing()
    add_executable(${TARGET_NAME} ${TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/NGIN/main.cpp)

    target_compile_definitions(${TARGET_NAME} PRIVATE -DNGIN_TESTING)
    target_link_libraries(${TARGET_NAME} PRIVATE gtest_main NGIN)

    # Register tests
    add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})

    # After the NGIN target is built, glob for DLLs and copy them
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -DDST_DIR="$<TARGET_FILE_DIR:${TARGET_NAME}>"
        -DSRC_DIR="$<TARGET_FILE_DIR:NGIN>"
        -P ${CMAKE_SOURCE_DIR}/CMakeCopyDll.cmake
    )

    # Run test automatically and show in output
    if(NIGN_TEST_ON_BUILD)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            add_custom_command(TARGET ${TARGET_NAME}
                            POST_BUILD
                            COMMAND $<TARGET_FILE:${TARGET_NAME}>)
        endif()
    endif()
endmacro()

ngin_add_test_macro(NGIN_LOGGIN_TESTS "Logging")
ngin_add_test_macro(NGIN_MEMORY_TESTS "Memory")
ngin_add_test_macro(NGIN_META_TESTS "Memory")

#[===[

# Define test sources
file(GLOB_RECURSE NGIN_TEST_SOURCES "*.cpp")

add_executable(NGIN_TESTS ${NGIN_TEST_SOURCES})

target_compile_definitions(NGIN PRIVATE -DNGIN_TESTING)
target_link_libraries(NGIN_TESTS PRIVATE gtest_main NGIN)

# Register tests

add_test(NAME NGIN_TESTS 
         COMMAND NGIN_TESTS)



# After the NGIN target is built, glob for DLLs and copy them
add_custom_command(TARGET NGIN_TESTS POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -DDST_DIR="$<TARGET_FILE_DIR:NGIN_TESTS>"
    -DSRC_DIR="$<TARGET_FILE_DIR:NGIN>"
    -P ${CMAKE_SOURCE_DIR}/CMakeCopyDll.cmake
)

    #run test automatically and show in output
if(NIGN_TEST_ON_BUILD)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET NGIN_TESTS
                        POST_BUILD
                        COMMAND $<TARGET_FILE:NGIN_TESTS>)
    endif()
endif()

]===]