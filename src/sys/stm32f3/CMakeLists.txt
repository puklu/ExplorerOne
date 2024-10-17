# # List of source files
# set(SYS_STM32F3_SOURCES
#     syscalls.c
#     sysmem.c
# )

# # Add these files to a library
# add_library(sys_stm32f3 ${SYS_STM32F3_SOURCES})

# # add_library(sys_stm32f3 STATIC ${SYS_STM32F3_SOURCES})
# # set_target_properties(sys_stm32f3 PROPERTIES LINKER_LANGUAGE C)

# # Other libraries needed
# target_link_libraries(sys_stm32f3 
#     PRIVATE 
# )

# # Include directories
# target_include_directories(sys_stm32f3
#     PUBLIC                        # make these include paths available others that link to this library
#         ${CMAKE_SOURCE_DIR}/src
# )
