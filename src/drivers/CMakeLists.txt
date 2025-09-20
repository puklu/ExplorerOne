# Define a variable for the platform (can be passed via -DPLATFORM=stm32f3discovery)
if(NOT DEFINED PLATFORM OR PLATFORM STREQUAL "")
    set(PLATFORM "stm32f3discovery" CACHE STRING "Target platform")
endif()

message(STATUS "Root CMakeLists: PLATFORM is set to '${PLATFORM}'")

# Add subdirectories
add_subdirectory(factory)
add_subdirectory(interfaces)
add_subdirectory(motion)
add_subdirectory(sensors)


# Add the platform-specific driver dynamically
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM}/CMakeLists.txt")
    add_subdirectory(${PLATFORM})
else()
    message(FATAL_ERROR "Platform '${PLATFORM}' not found in drivers directory.")
endif()


# Create an umbrella "drivers" library that links the component libraries
add_library(drivers INTERFACE)

target_link_libraries(drivers
    INTERFACE
        factory
        interfaces
        motion
        printf
        sensors
        ${PLATFORM}  # Link platform-specific driver
)
