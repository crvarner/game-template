cmake_minimum_required(VERSION 3.20)
project(GameTemplate    VERSION 1.0.0
                        LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)

# download all submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Checkout submodules during configure" ON)
    if(GIT_SUBMODULE)
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL 0)
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# check for required submodules
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/SDL/CMakeLists.txt")
    message(FATAL_ERROR "The SDL submodule was not downloaded. GIT_SUBMODULE was turned off or failed.")
endif()



add_subdirectory(F)
add_subdirectory(external/SDL)

target_link_libraries(${PROJECT_NAME}
    PRIVATE F
    PRIVATE SDL2)

