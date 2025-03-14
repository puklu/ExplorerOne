find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format) 

# Specify files to run clang-format on
set(FORMAT_DIRECTORIES 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)


file(GLOB_RECURSE FORMAT_FILES
    ${FORMAT_DIRECTORIES}/*.hpp
    ${FORMAT_DIRECTORIES}/*.cpp
)

# Function to run clang-format
add_custom_target(
    format
    COMMAND ${CLANG_FORMAT_EXECUTABLE}
    -i ${FORMAT_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format..."
)

# Add another command to print a success message after its done
add_custom_command(TARGET format
POST_BUILD
COMMENT "Formatting complete."
)

## Add a target to check code formatting (no -i flag)
# add_custom_target(
#     check-format
#     COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run -Werror ${SOURCES}
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     COMMENT "Checking code format..."
# )

## Add another command to print a success message after its done
# add_custom_command(TARGET check-format
# POST_BUILD
# COMMENT "Checking of format complete."
# )
