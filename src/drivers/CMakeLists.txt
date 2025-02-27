# Add subdirectories
add_subdirectory(interfaces)
add_subdirectory(stm32f3discovery)
add_subdirectory(motion)

# Create an umbrella "drivers" library that links the component libraries
add_library(drivers INTERFACE)

target_link_libraries(drivers
    INTERFACE
        common
        interfaces
        motion
        printf
        stm32f3discovery
)
