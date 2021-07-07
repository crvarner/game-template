cmake_minimum_required(VERSION 3.20)
project(GameTemplate    VERSION 1.0.0
                        LANGUAGES CXX)


# add main executable (entry point + SOURCES)
# in windows, WIN32 property adds /SUBSYSTEM:WINDOWS compiler flag 
# required for GUI applications
file(GLOB_RECURSE SOURCES src/*)
add_executable(${PROJECT_NAME} WIN32 app/main.cpp ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

# initialize lists of binaries to be installed
set(BINARIES ${PROJECT_NAME})
set(PREBUILT "")




# SDL (shared)
set(SDL_STATIC OFF CACHE BOOL "build SDL static lib" FORCE)
set(SDL_CMAKE_DEBUG_POSTFIX "" CACHE STRING "Debug DLL suffix" FORCE)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
target_include_directories(${PROJECT_NAME} PRIVATE external/SDL/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main SDL2)
if(WIN32)
    set_target_properties(SDL2 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()
list(APPEND BINARIES SDL2)








if(WIN32)
    # SDL_image build is broken for win32 so use precompiled binaries instead
    set(SDL_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_image-VC")
    set(SDL_IMAGE_LIB_DIR "${SDL_IMAGE_DIR}/lib/x64")

    add_library(SDL_image SHARED IMPORTED)
    set_target_properties(SDL_image PROPERTIES
        IMPORTED_LOCATION "${SDL_IMAGE_LIB_DIR}/SDL2_image.dll"
        IMPORTED_IMPLIB "${SDL_IMAGE_LIB_DIR}/SDL2_image.lib")

    add_library(png SHARED IMPORTED)
    set_property(TARGET png PROPERTY IMPORTED_LOCATION "${SDL_IMAGE_LIB_DIR}/libpng16-16.dll")
    
    add_library(zlib SHARED IMPORTED)
    set_property(TARGET zlib PROPERTY IMPORTED_LOCATION "${SDL_IMAGE_LIB_DIR}/zlib1.dll")

    add_library(jpeg SHARED IMPORTED)
    set_property(TARGET jpeg PROPERTY IMPORTED_LOCATION "${SDL_IMAGE_LIB_DIR}/jpeg-9.dll")

    target_include_directories(${PROJECT_NAME} PRIVATE "${SDL_IMAGE_DIR}/include")
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL_image)

    file(GLOB SDL_IMAGE_DLLS "${SDL_IMAGE_LIB_DIR}/*.dll")
    list(APPEND PREBUILT ${SDL_IMAGE_DLLS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL_IMAGE_DLLS}
        "${CMAKE_BINARY_DIR}/$<CONFIG>")
    
    # set(SDL_IMAGE_BASE "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_image-VC")
    # set(SDL_IMAGE_INCLUDE_DIRS "${SDL_IMAGE_BASE}/include")
    # set(SDL_IMAGE_LIBRARY_DIRS "${SDL_IMAGE_BASE}/lib/x64")
    # file(GLOB SDL_IMAGE_DLLS "SDL_IMAGE_LIBRARY_DIRS/*.dll")

    # target_include_directories(${PROJECT_NAME} PRIVATE "${SDL_IMAGE_INCLUDE_DIRS}")
    # target_link_directories(${PROJECT_NAME} PRIVATE "${SDL_IMAGE_LIBRARY_DIRS}")
    # target_link_libraries(${PROJECT_NAME} PRIVATE "SDL2_image.lib")    

    # set(SDL_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_image-VC")
    # target_include_directories(${PROJECT_NAME} PRIVATE "${SDL_IMAGE_DIR}/include")
    # target_link_directories(${PROJECT_NAME} PRIVATE "${SDL_IMAGE_DIR}/lib/x64")
    # target_link_libraries(${PROJECT_NAME} PRIVATE "SDL2_image.lib")
    # add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #     "${SDL_IMAGE_DIR}/lib/x64/*.dll"
    #     "${CMAKE_BINARY_DIR}")
else()
    add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
    target_include_directories(${PROJECT_NAME} PRIVATE external/SDL_image)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)
    list(APPEND BINARIES SDL2_image png zlib jpeg)
endif()






# copy asset dir to build dir
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets")
add_dependencies(${PROJECT_NAME} copy_assets)





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