cmake_minimum_required(VERSION 3.10)
project(Monobot)

set(CMAKE_CXX_STANDARD 17)

add_library(Monobot SHARED src/dllmain.cpp)

target_include_directories(Monobot PRIVATE include)
