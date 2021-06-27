cmake_minimum_required(VERSION 3.20)
project(GameTemplate    VERSION 1.0.0
                        LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(F)
add_subdirectory(external/SDL)

target_link_libraries(${PROJECT_NAME}
    PRIVATE F
    PRIVATE SDL2)

