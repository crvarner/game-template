cmake_minimum_required(VERSION 3.20)
project(GameTemplate    VERSION 1.0.0
                        LANGUAGES CXX)

file(GLOB_RECURSE SOURCES src/*)
add_executable(GameTemplate ${SOURCES})

# find packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# libraries
set(LIBRARIES
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES})

# include dirs
set(INCLUDE_DIRS
    include
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR})

target_include_directories( GameTemplate    PRIVATE     ${INCLUDE_DIRS})
target_link_libraries(      GameTemplate    PRIVATE     ${LIBRARIES})

