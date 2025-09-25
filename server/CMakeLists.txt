cmake_minimum_required(VERSION 3.15)
project(boost_asio_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake to use MinGW compiler
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# Find Boost installed by MSYS2
set(BOOST_ROOT "/mingw64")
find_package(Boost REQUIRED)

add_executable(server server.cpp)
target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(server
    Boost::boost
    ws2_32
    mswsock
)
