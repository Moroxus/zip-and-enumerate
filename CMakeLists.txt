cmake_minimum_required(VERSION 3.5)

project(zip-and-enumerate)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_executable(${PROJECT_NAME} main.cpp enumerate.hpp zip.hpp utility.hpp)
