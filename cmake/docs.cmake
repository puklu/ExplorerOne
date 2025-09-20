find_program(DOXYGEN_EXECUTABLE NAMES doxygen) 

if(NOT DOXYGEN_EXECUTABLE)
    message(FATAL_ERROR "Doxygen not found! Please install it.")
endif()


set(DOXYFILE ${CMAKE_SOURCE_DIR}/docs/Doxyfile)

if (NOT EXISTS ${DOXYFILE})
    message(FATAL_ERROR "Doxyfile not found at ${DOXYFILE}")
endif()

message("Doxyfile found at ${DOXYFILE}")

add_custom_target(doxygen
COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMENT "Generating API documentation with Doxygen"
)

# Add another command to print a success message after the documentation is generated
add_custom_command(TARGET doxygen
POST_BUILD
COMMENT "Doxygen documentation generation complete."
)
