cmake_minimum_required(VERSION 3.20)
project(GameTemplate    VERSION 1.0.0
                        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)


# download all submodules if missing
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL 0)
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()


# add main executable (entry point + SOURCES)
# in windows, WIN32 property adds /SUBSYSTEM:WINDOWS compiler flag 
# required for GUI applications
file(GLOB_RECURSE SOURCES src/*)
add_executable(${PROJECT_NAME} WIN32 app/main.cpp ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)


# initialize list of binaries to be installed
set(BINARIES ${PROJECT_NAME})


# SDL
set(SDL_STATIC OFF CACHE BOOL "build SDL static lib" FORCE)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
target_include_directories(${PROJECT_NAME} PRIVATE external/SDL/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main SDL2)
list(APPEND BINARIES SDL2)


# SDL_image
# disable jpeg support since windows build doesn't work with CMake (do symbols exported)
set(SUPPORT_JPG OFF CACHE BOOL "enable jpeg support in SDL_image" FORCE)
# add_subdirectory(external/SDL_image_fork EXCLUDE_FROM_ALL)
# target_include_directories(${PROJECT_NAME} PRIVATE external/SDL_image_fork)
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
target_include_directories(${PROJECT_NAME} PRIVATE external/SDL_image)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)
list(APPEND BINARIES SDL2_image png zlib)


# LuaJIT
add_subdirectory(external/LuaJIT-cmake EXCLUDE_FROM_ALL)
get_target_property(LUA_INCLUDE_DIR liblua-shared INCLUDE_DIRECTORIES)
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE liblua-shared)
if(APPLE)
    # says it was fixed in LuaJIT 2.1, but it doesn't work without it?
    target_link_options(${PROJECT_NAME}
        PRIVATE "-pagezero_size 10000 -image_base 100000000")
endif()
list(APPEND BINARIES liblua-shared)


# set shared lib output directories to match executable
set_target_properties(${BINARIES} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")


# copy asset dir to build dir
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets")
add_dependencies(${PROJECT_NAME} copy_assets)


# copy asset dir to build dir
add_custom_target(copy_scripts
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts"
    "${CMAKE_BINARY_DIR}/scripts")
add_dependencies(${PROJECT_NAME} copy_scripts)


#installation
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "./")
install(TARGETS ${BINARIES} DESTINATION .)
install(FILES ${PREBUILT} DESTINATION .)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION .)

# CPack common settings
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${GameTemplate_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${GameTemplate_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_MINOR ${GameTemplate_VERSION_PATCH})

if(APPLE)
    # generate mac package files
    set(STARTUP_SCRIPT "${CMAKE_BINARY_DIR}/start.sh")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/start.sh.in" "${STARTUP_SCRIPT}")
    set(INFO_PLIST "${CMAKE_BINARY_DIR}/Info.plist")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Info.plist.in" "${INFO_PLIST}")

    # set CPack bundle variables
    set(CPACK_GENERATOR "Bundle")
    set(CPACK_BUNDLE_NAME ${PROJECT_NAME})
    set(CPACK_BUNDLE_PLIST "${INFO_PLIST}")
    set(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AppIcon.icns")
    set(CPACK_BUNDLE_STARTUP_COMMAND "${STARTUP_SCRIPT}")
endif()

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")
endif()

include(CPack)