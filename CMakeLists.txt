cmake_minimum_required(VERSION 3.10)
project(adventurer VERSION 1.0.0 LANGUAGES CXX)

# Default to Release build for optimized performance
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags - add optimization flags for GNU/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3 -DNDEBUG)
    endif()
    add_compile_options(-Wall -Wextra -pedantic)
elseif(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2 /DNDEBUG)
    endif()
    add_compile_options(/W4)
endif()

# Find SDL2 package
find_package(SDL2 REQUIRED)

# Source files organized by subsystem
set(SOURCES
    # Core engine
    src/main.cpp
    src/Game.cpp
    src/InputManager.cpp
    src/Camera.cpp
    
    # Graphics
    graphics/ProceduralArt.cpp
    
    # World/Tiles
    world/TileMap.cpp
    
    # Utilities
    utils/Settings.cpp
)

# Include directories for project headers
include_directories(${CMAKE_SOURCE_DIR})

# Create executable for Linux native build
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_executable(adventurer_linux ${SOURCES})
    target_link_libraries(adventurer_linux SDL2::SDL2)
    set_target_properties(adventurer_linux PROPERTIES OUTPUT_NAME adventurer_linux)
endif()

# Create executable for Windows cross-compilation
if(CMAKE_CROSSCOMPILING OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Consolidate Windows target definitions into a single, clean target
    add_executable(adventurer_windows ${SOURCES})
    
    # Correct order: link SDL2main first, then SDL2 static lib
    set(SDL2_LIB_DIR /usr/x86_64-w64-mingw32/lib)
    target_link_libraries(adventurer_windows 
        ${SDL2_LIB_DIR}/libSDL2main.a
        ${SDL2_LIB_DIR}/libSDL2.a
        -lsetupapi -limm32 -lwinmm -loleaut32 -luuid -lversion
        -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32
        -static-libgcc -static-libstdc++ -static
    )
    
    set_target_properties(adventurer_windows PROPERTIES 
        OUTPUT_NAME adventurer_windows
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        ENABLE_EXPORTS FALSE
        WINDOWS_EXPORT_ALL_SYMBOLS FALSE
    )
endif()

# Print configuration summary
message(STATUS "========================================")
message(STATUS "Adventurer Build Configuration")
message(STATUS "========================================")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "System Name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "SDL2 Found: ${SDL2_FOUND}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "========================================")
