find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format) 

# Specify files to run clang-format on
set(DIRECTORIES_TO_FORMAT 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)

foreach(dir ${DIRECTORIES_TO_FORMAT})
    file(GLOB_RECURSE FILE_IN_DIR
        ${dir}/*.hpp
        ${dir}/*.cpp
    )
    list(APPEND FILES_TO_FORMAT ${FILE_IN_DIR})
endforeach()

# Function to run clang-format
add_custom_target(
    format
    COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${FILES_TO_FORMAT}
    COMMAND ${CMAKE_COMMAND} -E echo "Formatting complete."
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format..."
    VERBATIM
)
