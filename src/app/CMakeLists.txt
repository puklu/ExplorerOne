# List of source files
set(APP_SOURCES
    Bot.cpp
    FSM.cpp
    StateBase.cpp
    StateMovingForward.cpp
    StateStopped.cpp
    StateTurningToLeft.cpp
    StateTurningToRight.cpp
    Transition.cpp
    UltrasonicSensorStub.cpp
)

# Add these files to a library
add_library(app ${APP_SOURCES})

# Other libraries needed
target_link_libraries(app 
    PRIVATE 
        common
        drivers
        printf
        stm
        sys_stm32f3
)

# Include directories
target_include_directories(app
    PUBLIC    # make these include paths available to others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)

# Startup file
set(START_UP_FILE "${CMAKE_SOURCE_DIR}/src/app/startup_stm32f303vctx.s")

add_executable(app.elf
    main.cpp
    ${START_UP_FILE}
)

# Link the libraries with the app executable
target_link_libraries(app.elf 
    PRIVATE
        app
        common
        drivers
        # io
        printf
        stm
        sys_stm32f3
)

#  Apply app-specific linker script
target_link_options(app.elf
    PRIVATE
        "-T${CMAKE_SOURCE_DIR}/src/app/app.ld"
        "-Wl,--gc-sections"
)

# Tell the linker to generate a map file for app
set_target_properties(app.elf PROPERTIES
    LINK_FLAGS "-Wl,-Map=app.map"
    COMMENT "Generating Map file for app"
)

# Convert ELF to binary for app
add_custom_command(TARGET app.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary app.elf app.bin
    COMMENT "Converting app ELF to BIN"
)

if(USE_LIBOPENCM3)
    target_link_libraries(app.elf PRIVATE libopencm3)
    target_compile_definitions(app.elf PRIVATE USE_LIBOPENCM3)
endif()

