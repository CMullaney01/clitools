cmake_minimum_required(VERSION 3.15)
project(clitools LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build the clitools library
add_subdirectory(src)

# Build the tools
add_subdirectory(tools/examples)
add_subdirectory(tools/todo)
