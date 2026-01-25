cmake_minimum_required(VERSION 3.15)
project(boost_raylib_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------
# Compiler (ONLY force on Windows / MinGW)
# -------------------------------------------------
if (WIN32)
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
endif()

# -------------------------------------------------
# Output directory
# -------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# -------------------------------------------------
# Boost
# -------------------------------------------------
if (WIN32)
    set(BOOST_ROOT "C:/msys64/mingw64")
    find_package(Boost REQUIRED COMPONENTS system)
else()
    # macOS: boost_system is header-only
    set(Boost_NO_BOOST_CMAKE ON)
    find_package(Boost REQUIRED)
endif()

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif()

# -------------------------------------------------
# Raylib
# -------------------------------------------------
if (WIN32)
    set(RAYLIB_PATH "C:/raylib/raylib")
    set(RAYLIB_INCLUDE_DIR "${RAYLIB_PATH}/src")
    set(RAYLIB_LIB_DIR "${RAYLIB_PATH}/src")
else()
    # macOS: raylib via Homebrew
    find_package(raylib REQUIRED)
endif()

# -------------------------------------------------
# Sources
# -------------------------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# -------------------------------------------------
# Executable
# -------------------------------------------------
add_executable(main ${SOURCES})

# -------------------------------------------------
# Includes
# -------------------------------------------------
target_include_directories(main PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/include
)

if (WIN32)
    target_include_directories(main PRIVATE ${RAYLIB_INCLUDE_DIR})
endif()

# -------------------------------------------------
# Linking
# -------------------------------------------------
if (WIN32)
    target_link_directories(main PRIVATE ${RAYLIB_LIB_DIR})

    target_link_libraries(main PRIVATE
        ${Boost_LIBRARIES}
        ws2_32
        pthread
        raylib
        opengl32
        gdi32
        winmm
    )

elseif (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)

    target_link_libraries(main PRIVATE
        raylib
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
        ${OPENGL_LIBRARY}
    )
endif()

# -------------------------------------------------
# Output name
# -------------------------------------------------
set_target_properties(main PROPERTIES
    OUTPUT_NAME "main"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
