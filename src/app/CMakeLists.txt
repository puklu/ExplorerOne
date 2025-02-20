# List of source files
set(APP_SOURCES
    Bot.cpp
    FSM.cpp
    State.cpp
    Transition.cpp
)

# Add these files to a library
add_library(app ${APP_SOURCES})

# Other libraries needed
target_link_libraries(app 
    PRIVATE 
        common
        # io
        stm
)

# Include directories
target_include_directories(app
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
