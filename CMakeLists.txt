cmake_minimum_required(VERSION 3.2)

project(milestone1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(BUILD_DIR ${PROJECT_SOURCE_DIR}/build)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

include_directories(${INCLUDE_DIR})

add_subdirectory(src)
