cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(file_data)

# Build target
add_executable(file_data main.cpp)
target_include_directories(file_data PRIVATE .)

# Link modules
target_include_directories(file_data PRIVATE ../../include)
