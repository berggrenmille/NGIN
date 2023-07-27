# add API documentation
 # add doxygen 

 # Add an option to control the building of documentation

    find_package(Doxygen)
    if(DOXYGEN_FOUND)

        # log doc build/install paths
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)
        set(DOXYGEN_IN "${CMAKE_SOURCE_DIR}/docs/Doxyfile.in")

        message(STATUS "Documentation will be built in ${DOXYGEN_OUT}")

        #set input and output files
        set(DOXYGEN_PROJECT_NAME, ${PROJECT_NAME})
        set(DOXYGEN_PROJECT_BRIEF,  ${PROJECT_DESCRIPTION})
        set(DOXYGEN_PROJECT_NUMBER, ${PROJECT_VERSION})
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
        set(DOXYGEN_TEMPLATE_RELATIONS YES)
        set(DOXYGEN_BUILTIN_STL_SUPPORT YES)


        #request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        doxygen_add_docs( docs ALL
            ${NGIN_SOURCE_DIR}/include
            ${NGIN_SOURCE_DIR}/src
            ${NGIN_EDITOR_SOURCE_DIR}/include
            ${NGIN_EDITOR_SOURCE_DIR}/src
          COMMENT
            "HTML documentation"
          )

  
    else()
        message("Doxygen need to be installed to generate the API documentation")
    endif()