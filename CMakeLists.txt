cmake_minimum_required(VERSION 3.16)
project(Flare VERSION 0.0.1 LANGUAGES CXX)

# Build configuration options
option(FLARE_BUILD_SANDBOX "Build the Sandbox executable" ON)
option(FLARE_BUILD_STATIC "Build Flare as a static library instead of shared" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Select a build type (Debug or Release)" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(Vendor/glfw)

# GLM
add_subdirectory(Vendor/glm)

# Box3D
add_subdirectory(Vendor/Box3D)

# Assimp
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_FORCE_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT FALSE CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_GLTF_IMPORTER TRUE CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE CACHE BOOL "" FORCE)

add_subdirectory(Vendor/Assimp)

# Flecs
add_subdirectory(Vendor/Flecs)

# Flare Library Configuration
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    Code/*.cpp
    Code/*.h
)

# Determine library type based on option
if(FLARE_BUILD_STATIC)
    set(FLARE_LIB_TYPE STATIC)
    message(STATUS "Building Flare as a STATIC library")
else()
    set(FLARE_LIB_TYPE SHARED)
    message(STATUS "Building Flare as a SHARED library")
endif()

add_library(Flare ${FLARE_LIB_TYPE} ${LIB_SOURCES})

target_include_directories(Flare PUBLIC
    Code/
    Vendor/Glad
    Vendor/stb
)

target_link_libraries(Flare PUBLIC glfw)
target_link_libraries(Flare PUBLIC glm::glm)
target_link_libraries(Flare PUBLIC box3d::box3d)
target_link_libraries(Flare PUBLIC assimp::assimp)
target_link_libraries(Flare PUBLIC flecs)

# Common compile definitions for Flare
target_compile_definitions(Flare PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

# Define static macro if built as static library
if(FLARE_BUILD_STATIC)
    target_compile_definitions(Flare PUBLIC FLARE_STATIC)
endif()

# Conditional Sandbox Build
if(FLARE_BUILD_SANDBOX)
    file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS
        Sandbox/*.cpp
        Sandbox/*.h
    )

    add_executable(Sandbox ${SANDBOX_SOURCES})

    target_link_libraries(Sandbox PRIVATE Flare)

    target_compile_definitions(Sandbox PUBLIC
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
    )
endif()