cmake_minimum_required(VERSION 3.5)

project(hello_cmake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(boost-hello)
add_subdirectory(say-hello)
add_subdirectory(hello-exe)
