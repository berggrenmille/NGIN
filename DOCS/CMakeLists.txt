# add API documentation
 # add doxygen 

 # Add an option to control the building of documentation

    find_package(Doxygen)

    if(DOXYGEN_FOUND)


        set(DOXYGEN_CSS "${CMAKE_SOURCE_DIR}/DOCS/doxygen-awesome.css")
        set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
        set(DOXYGEN_IN "${CMAKE_SOURCE_DIR}/DOCS/Doxyfile.in")
        set(DOXYGEN_INPUT_DIRS "${NGIN_SOURCE_DIR}")

        message(STATUS "Documentation will be built in ${DOXYGEN_OUT}")
        
        configure_file(${DOXYGEN_CSS} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        add_custom_target( DOCS ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )



  
    else()
      message(WARNING "Doxygen not found. Documentation will not be generated.")
    endif()