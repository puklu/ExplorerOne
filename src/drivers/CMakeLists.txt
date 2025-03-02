# Add subdirectories
add_subdirectory(factory)
add_subdirectory(interfaces)
add_subdirectory(motion)
add_subdirectory(stm32f3discovery)

# Create an umbrella "drivers" library that links the component libraries
add_library(drivers INTERFACE)

target_link_libraries(drivers
    INTERFACE
        factory
        interfaces
        motion
        printf
        stm32f3discovery
)
