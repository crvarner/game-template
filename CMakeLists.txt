cmake_minimum_required(VERSION 3.20)
project(GameTemplate    VERSION 1.0.0
                        LANGUAGES CXX)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOS TRUE)
endif()

file(GLOB_RECURSE SOURCES src/*)
add_executable(${PROJECT_NAME} app/main.cpp ${SOURCES})

# find dynamically linked libs
find_package(SDL2 REQUIRED)

# includes
target_include_directories(GameTemplate
    PRIVATE src
    PRIVATE ${SDL2_INCLUDE_DIRS})

# linker
target_link_libraries(GameTemplate
    PRIVATE ${SDL2_LIBRARIES})

#installation
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# CPack stuff
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${GameTemplate_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${GameTemplate_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_MINOR ${GameTemplate_VERSION_PATCH})

if(APPLE)
    set(STARTUP_SCRIPT "${CMAKE_BINARY_DIR}/start.sh")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/start.sh.in" "${STARTUP_SCRIPT}")

    set(CPACK_GENERATOR "Bundle")
    set(CPACK_BUNDLE_NAME ${PROJECT_NAME})
    set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Info.plist")
    set(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Info.plist")
    set(CPACK_BUNDLE_STARTUP_COMMAND "${STARTUP_SCRIPT}")
endif()

include(CPack)



































# download all submodules
# find_package(Git QUIET)
# if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
#     option(GIT_SUBMODULE "Checkout submodules during configure" ON)
#     if(GIT_SUBMODULE)
#         execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#                         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                         RESULT_VARIABLE GIT_SUBMOD_RESULT)
#         if(NOT GIT_SUBMOD_RESULT EQUAL 0)
#             message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#         endif()
#     endif()
# endif()

# check for required submodules
# if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/SDL/CMakeLists.txt")
#     message(FATAL_ERROR "The SDL submodule was not downloaded. GIT_SUBMODULE was turned off or failed.")
# endif()