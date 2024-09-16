find_program(DOXYGEN_EXECUTABLE NAMES doxygen) 

if (DOXYGEN_EXECUTABLE)
    set(DOXYFILE ${CMAKE_SOURCE_DIR}/docs/Doxyfile)

    add_custom_target(run-doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    )

    # Add another command to print a success message after the documentation is generated
    add_custom_command(TARGET run-doxygen
    POST_BUILD
    COMMENT "Doxygen documentation generation complete."
    )

else()
    message(STATUS "Doxygen not found, skipping documentation generation.")
endif()
