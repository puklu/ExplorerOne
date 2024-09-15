cmake_minimum_required(VERSION 3.10)
project(ExplorerOne C CXX ASM)

# Specify the target chip
set(CHIP "STM32F303xC")

# Include complier and linker flags settings
include(cmake/flags.cmake)

# Add subdirectory for drivers
add_subdirectory(src/drivers)

# Add subdirectory for src
add_subdirectory(src)

# Include formatting and cppcheck
include(cmake/formatting.cmake)
include(cmake/cppcheck.cmake)

add_executable(${PROJECT_NAME}.elf src/main.cpp)

# Link the drivers library to the main add_executable
target_link_libraries(${PROJECT_NAME}.elf PRIVATE main drivers)

# Convert ELF to binary
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Converting ELF to BIN"
)

add_dependencies(${PROJECT_NAME}.elf clang-format)
add_dependencies(${PROJECT_NAME}.elf run_cppcheck)
