# Define a variable for the LOG_LEVEL (can be passed via -DLOG_LEVEL=3)
if(NOT DEFINED LOG_LEVEL)
    set(LOG_LEVEL 3)
endif()

# Pass LOG_LEVEL to the compiler as a macro
add_compile_definitions(LOG_LEVEL=${LOG_LEVEL})

message(STATUS "Logging level: ${LOG_LEVEL}")
