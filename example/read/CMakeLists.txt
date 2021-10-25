cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(read)

# Build target
add_executable(read main.cpp)
target_include_directories(read PRIVATE .)

# Link modules
target_include_directories(read PRIVATE ../../include)
