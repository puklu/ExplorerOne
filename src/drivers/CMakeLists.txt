# List of source files
set(DRIVERS_SOURCES
    io/GPIOpin.cpp
    mcu/mcuInit.cpp
)

# Add these files to a library
add_library(drivers ${DRIVERS_SOURCES})

# Set the path to the CMSIS and STM32 headers
set(CMSIS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stm/CMSIS")
set(STM32F3XX_DIR "${CMSIS_DIR}/Device/ST/STM32F3xx")

# Other libraries needed
target_link_libraries(drivers PRIVATE common)

# Include directories
target_include_directories(drivers 
    # PRIVATE   # TODO: Uncomment this
    PUBLIC                      # make these include paths available only to this library
        ${CMSIS_DIR}/Include      # To be able just use the stm header without typing complete path
        ${STM32F3XX_DIR}/Include
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_CURRENT_SOURCE_DIR}
)
