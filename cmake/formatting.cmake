find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format) 

# Specify files to run clang-format on
set(FORMAT_DIRECTORIES 
    src/
)

file(GLOB_RECURSE FORMAT_FILES
    ${FORMAT_DIRECTORIES}/*.hpp
    ${FORMAT_DIRECTORIES}/*.cpp
)

# Function to run clang-format
add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT_EXECUTABLE}
    -i ${FORMAT_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format..."
)

# Add another command to print a success message after its done
add_custom_command(TARGET clang-format
POST_BUILD
COMMENT "Formatting complete."
)

## Add a target to check code formatting (no -i flag)
# add_custom_target(
#     clang-format-check
#     COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run -Werror ${SOURCES}
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     COMMENT "Checking code format..."
# )

## Add another command to print a success message after its done
# add_custom_command(TARGET clang-format-check
# POST_BUILD
# COMMENT "Checking of format complete."
# )
