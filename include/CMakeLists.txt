file(GLOB_RECURSE CHIP8_INCLUDE_FILES *.h)
add_library(CHIP8_INCLUDE ${CHIP8_INCLUDE_FILES})

set_target_properties(CHIP8_INCLUDE PROPERTIES LINKER_LANGUAGE C)

target_include_directories(CHIP8_INCLUDE PUBLIC "${PROJECT_SOURCE_DIR}/include"
                                                "${PROJECT_SOURCE_DIR}/src"
                                                )