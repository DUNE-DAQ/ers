
cmake_minimum_required(VERSION 3.12)
project(ers)

set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_NO_BOOST_CMAKE ON)
set(BUILD_SHARED_LIBS ON)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(bin)
add_subdirectory(src)
add_subdirectory(test)
