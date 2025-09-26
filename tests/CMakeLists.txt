# Ensure TEST_NAME is defined
if(NOT DEFINED TEST_NAME)
    message(FATAL_ERROR "TEST_NAME not specified. Please set -DTEST_NAME=<test_name>")
endif()

if(NOT DEFINED USE_LIBOPENCM3)
    set(USE_LIBOPENCM3 OFF CACHE BOOL "Use libopencm3")
endif()

# List of source files
set(OTHER_SOURCES
)

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.cpp
)

# Check that source file exists
if(NOT EXISTS ${TEST_SOURCES})
    message(FATAL_ERROR "Test source file ${TEST_NAME}.cpp does not exist!")
endif()    

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

add_executable(${TEST_NAME}.elf  ${START_UP_FILE} ${TEST_SOURCES} ${OTHER_SOURCES})

# Link the drivers library with the executable
target_link_libraries(${TEST_NAME}.elf
    PRIVATE
        common
        drivers
        sys_stm32f3
)

if(USE_LIBOPENCM3)
    target_link_libraries(${TEST_NAME}.elf PRIVATE libopencm3)
    target_compile_definitions(${TEST_NAME}.elf PRIVATE USE_LIBOPENCM3)
endif()

message(%%%%%%%%%%%)
message(${USE_LIBOPENCM3})

# Include directories
target_include_directories(${TEST_NAME}.elf
                           PUBLIC 
                           ${SRC_DIR}
)

# Add custom commands to convert ELF to BIN for each test
add_custom_command(TARGET ${TEST_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${TEST_NAME}.elf ${TEST_NAME}.bin
    COMMENT "Converting ${TEST_NAME}.elf to BIN"
)
