# Define the api library
add_library(vl53l1x_api STATIC)


# DO NOT INCLUDE platform/src files! We have our own STM32 platform implementation.
target_sources(vl53l1x_api 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_api.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_api_calibration.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_api_core.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_api_debug.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_api_preset_modes.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_api_strings.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_core.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_core_support.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_error_strings.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_register_funcs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_silicon_core.c
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/src/vl53l1_wait.c
)

# Include the current directory for header files
target_include_directories(vl53l1x_api 
    SYSTEM PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/API/core/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/API/platform/inc
)

target_compile_options(vl53l1x_api PRIVATE
    # -Wno-error=cpp   # Don't treat #warning as error
    # OR:
    -Wno-error      # Don't treat ANY warnings as errors
)

set_target_properties(vl53l1x_api PROPERTIES LINKER_LANGUAGE C)
