cmake_minimum_required(VERSION 3.20)
project(Flare VERSION 0.0.1 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Select a build type (Debug or Release)" FORCE)
endif()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GLFW

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(Vendor/glfw)

# Flare

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Code/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Code/*.h"
)

add_library(Flare SHARED ${LIB_SOURCES})

target_include_directories(Flare PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Code")

target_link_libraries(Flare PUBLIC glfw)

#Sandbox

file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Sandbox/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sandbox/*.h"
)

add_executable(Sandbox ${SANDBOX_SOURCES})

target_link_libraries(Sandbox PRIVATE Flare)

# Compile defs

target_compile_definitions(Flare PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

target_compile_definitions(Sandbox PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)
