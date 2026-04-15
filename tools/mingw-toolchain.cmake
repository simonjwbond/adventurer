# MinGW-w64 Cross-Compilation Toolchain File for Adventurer
# Usage: cmake -DCMAKE_TOOLCHAIN_FILE=../tools/mingw-toolchain.cmake ..

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10)

# Use the posix variant which uses main() as entry point (not WinMain)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++-posix)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-posix)
set(CMAKE_AR x86_64-w64-mingw32-ar)
set(CMAKE_RANLIB x86_64-w64-mingw32-ranlib)

# Force console subsystem BEFORE any targets are created
set(CMAKE_EXE_LINKER_FLAGS "-mconsole")
set(CMAKE_WIN32_EXECUTABLE FALSE)

# Target environment settings
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# SDL2 for MinGW
set(SDL2_DIR /usr/lib/x86_64-w64-mingw32/cmake/SDL2)

# Compiler flags for Windows
add_compile_options(-Wall -Wextra)

message(STATUS "Configuring for Windows x64 cross-compilation")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
