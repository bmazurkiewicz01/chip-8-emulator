file(GLOB_RECURSE CHIP8_SOURCE_FILES *.c)
list(FILTER CHIP8_SOURCE_FILES EXCLUDE REGEX ".*main.c$")
add_library(CHIP8_SOURCE ${CHIP8_SOURCE_FILES})

set_target_properties(CHIP8_SOURCE PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(CHIP8_SOURCE "CHIP8_INCLUDE"
                                      ${SDL2_LIBRARIES})

target_include_directories(CHIP8_SOURCE PUBLIC "${PROJECT_SOURCE_DIR}/include"
                                                "${PROJECT_SOURCE_DIR}/src"
                                                )