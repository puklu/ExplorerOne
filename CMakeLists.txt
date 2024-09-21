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

# Include formatting
include(cmake/formatting.cmake)

# Include cppcheck
include(cmake/cppcheck.cmake)

# Include generation of documentation
include(cmake/docs.cmake)

add_dependencies(${PROJECT_NAME}.elf clang-format)
add_dependencies(${PROJECT_NAME}.elf run_cppcheck)
