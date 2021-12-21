cmake_minimum_required(VERSION 3.5)

project(boost_hello C CXX)

add_library(boost-hello INTERFACE
    src/boost-hello/boost-hello.hpp
)
target_include_directories(boost-hello INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")

