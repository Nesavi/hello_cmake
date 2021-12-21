cmake_minimum_required(VERSION 3.5)

project(hello C CXX)

set(Boost_DETAILED_FAILURE_MSG ON)
set(BOOST_ROOT "C:/Users/thomas.lassen/source/boost/boost_1_77_0")
#set(BOOST_LIBRARYDIR "C:/Users/thomas.lassen/source/boost/boost_1_77_0/stage/lib")
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
set(Boost_NO_SYSTEM_PATHS ON)
find_package(Boost 1.77.0 COMPONENTS serialization REQUIRED)

message(STATUS ${Boost_INCLUDE_DIR})
message(STATUS ${Boost_INCLUDE_DIRS})
message(STATUS ${Boost_LIBRARY_DIRS})
message(STATUS Boost_FOUND)

#include_directories(${Boost_INCLUDE_DIRS})

add_library(lib3 STATIC src_lib3.h src_lib3.cpp)
add_executable(hello_cmake main.cpp src1.h src2_lib2.h src2_lib2.cpp)
target_include_directories(hello_cmake PUBLIC ${Boost_INCLUDE_DIRS})
target_link_directories(hello_cmake PUBLIC ${Boost_LIBRARY_DIRS})
target_link_libraries(hello_cmake PRIVATE lib3)
target_link_libraries(hello_cmake PUBLIC say-hello)
target_link_libraries(hello_cmake PUBLIC boost-hello)
target_link_libraries(hello_cmake PUBLIC Boost::serialization)