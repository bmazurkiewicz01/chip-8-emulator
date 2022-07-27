cmake_minimum_required(VERSION 3.7)

project(chip_8_emulator LANGUAGES C VERSION 1.0.0)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/src/main.c")

add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} "CHIP8_INCLUDE"
                                      "CHIP8_SOURCE"
                                      ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC
                            "${PROJECT_SOURCE_DIR}/src"
                            "${PROJECT_SOURCE_DIR}/include"
                            "${SDL2_INCLUDE_DIRS}")
