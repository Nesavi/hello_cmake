cmake_minimum_required(VERSION 3.5)

MESSAGE(STATUS "PROCESSING LIB boost-hello")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(Boost_DETAILED_FAILURE_MSG ON)
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../boost/boost_1_77_0/")
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

add_library(boost-hello STATIC src/boost-hello/boost-hello.cpp)
target_include_directories(boost-hello PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
# target_include_directories(boost-hello PUBLIC ${Boost_INCLUDE_DIRS})
# target_link_directories(boost-hello PUBLIC ${Boost_LIBRARY_DIRS})
target_link_libraries(boost-hello PUBLIC Boost::serialization)