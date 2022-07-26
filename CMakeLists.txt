cmake_minimum_required(VERSION 3.5)
project(chip_8_emulator LANGUAGES C VERSION 1.0.0)

add_compile_options(-lSDL2)

add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/src/main/main.c")

add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} CHIP8_INCLUDE)
target_include_directories(${PROJECT_NAME} PUBLIC
                            "${PROJECT_SOURCE_DIR}/src/main"
                            "${PROJECT_SOURCE_DIR}/include")
