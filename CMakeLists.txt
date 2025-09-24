cmake_minimum_required(VERSION 3.15)
project(boost_raylib_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# --- Compilers (for MinGW on Windows) ---
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# --- Boost ---
set(BOOST_ROOT "C:/msys64/mingw64")
find_package(Boost REQUIRED COMPONENTS system)

# --- Raylib ---
set(RAYLIB_PATH "C:/raylib/raylib")                # Change to your raylib path
set(RAYLIB_INCLUDE_DIR "${RAYLIB_PATH}/src")
set(RAYLIB_LIB_DIR "${RAYLIB_PATH}/src")

# --- Collect all source files from src/ only ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
add_executable(main ${SOURCES})

# --- Include directories ---
target_include_directories(main PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${RAYLIB_INCLUDE_DIR}
)

# --- Link libraries ---
target_link_directories(main PRIVATE ${RAYLIB_LIB_DIR})
target_link_libraries(main PRIVATE
    ${Boost_LIBRARIES}   # Boost system
    ws2_32               # Windows sockets
    pthread              # Threads
    raylib               # Raylib
    opengl32
    gdi32
    winmm
)
