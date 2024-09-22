find_program(CPPCHECK_EXECUTABLE NAMES cppcheck)

# Enable compile_commands.json for better cppcheck analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(CPPCHECK_EXECUTABLE)
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE}
        --enable=all                            # enable all checks
        --inconclusive
        --quiet
        --suppress=missingInclude
        --suppress=missingIncludeSystem
        --suppress=unusedFunction
        --suppress=checkersReport
        ${CMAKE_SOURCE_DIR}/src/                # direcotry to include for check
        -i${CMAKE_SOURCE_DIR}/src/drivers/stm   # directories to ignore for check
        # --project=${CMAKE_BINARY_DIR}/compile_commands.json # Use compile_commands.json for flags
        -D__GNUC__                              # Define GNU C compiler
        --verbose
        COMMENT "Running static analysis! Cppcheck found at: ${CPPCHECK_EXECUTABLE}"
    )

    # Add another command to print a success message after its done
    add_custom_command(TARGET cppcheck
    POST_BUILD
    COMMENT "Static analysis complete."
    )

else()
    message(WARNING "Cppcheck not found. Skipping static analysis.")    
endif()  
