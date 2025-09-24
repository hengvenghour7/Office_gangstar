cmake_minimum_required(VERSION 3.15)
project(boost_raylib_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# --- Compilers (for MinGW on Windows) ---
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# --- Set global output for executables to project root ---
# All executables will go here
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# --- Boost setup ---
set(BOOST_ROOT "C:/msys64/mingw64")
find_package(Boost REQUIRED COMPONENTS system)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif()

# --- Raylib setup ---
set(RAYLIB_PATH "C:/raylib/raylib")  # Adjust your path
set(RAYLIB_INCLUDE_DIR "${RAYLIB_PATH}/src")
set(RAYLIB_LIB_DIR "${RAYLIB_PATH}/src")

# --- Collect all source files from src/ ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# --- Create executable ---
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

# --- Force executable name to main.exe ---
set_target_properties(main PROPERTIES
    OUTPUT_NAME "main"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
