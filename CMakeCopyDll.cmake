# copy_dlls.cmake

# Grab the arguments (source and destination directories)


message(STATUS "src_dir: ${SRC_DIR}")
message(STATUS "dst_dir: ${DST_DIR}")


# Glob for .dll files in the source directory
file(GLOB dll_files "${SRC_DIR}/*.dll")

# Loop over the .dll files and copy each one
foreach(file ${dll_files})
    file(COPY ${file} DESTINATION ${DST_DIR})
endforeach()
