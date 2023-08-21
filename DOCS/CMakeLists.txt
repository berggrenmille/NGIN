# Adding support for API documentation using Doxygen

# Search for the Doxygen package in the system
find_package(Doxygen)

# Check if Doxygen is found
if(DOXYGEN_FOUND)
    # Set paths for Doxygen output and input files
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    set(DOXYGEN_OUT_HTML "${CMAKE_CURRENT_BINARY_DIR}/html")
    set(DOXYGEN_IN "${CMAKE_SOURCE_DIR}/DOCS/Doxyfile.in")
    set(DOXYGEN_INPUT_DIRS "${NGIN_SOURCE_DIR}")

    # Define paths for custom Doxygen CSS and Markdown files
    set(DOXYGEN_CSS "doxygen-awesome.css")
    set(DOXYGEN_MD "${DOXYGEN_OUT_HTML}/index.md")
    
    # Display a message indicating where the documentation will be built
    message(STATUS "Documentation will be built in ${DOXYGEN_OUT}")

    # Copy custom CSS to the HTML output folder
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_CSS}" "${DOXYGEN_OUT_HTML}/${DOXYGEN_CSS}" COPYONLY)

    # Copy custom Markdown to the HTML output folder
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/index.md" "${DOXYGEN_MD}" @ONLY)

    # Configure the Doxyfile (Doxygen configuration file) using the provided template
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Add a custom build target to generate the API documentation using Doxygen
    add_custom_target(
        DOCS ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    # Display a warning message if Doxygen is not found
    message(WARNING "Doxygen not found. Documentation will not be generated.")
endif()
