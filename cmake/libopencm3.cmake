# create an INTERFACE target for libopencm3
add_library(libopencm3 INTERFACE)

# provide include path
target_include_directories(libopencm3 
    INTERFACE
        ${CMAKE_SOURCE_DIR}/external/libopencm3/include
)

# link against the prebuilt opencm3 archive (must be built manually with `make`)
target_link_libraries(libopencm3
    INTERFACE
        ${CMAKE_SOURCE_DIR}/external/libopencm3/lib/libopencm3_stm32f3.a
)

# # relax warning only when compiling code that includes libopencm3
# target_compile_options(libopencm3
#     INTERFACE
#         -Wno-error=macro-redefined
# )
