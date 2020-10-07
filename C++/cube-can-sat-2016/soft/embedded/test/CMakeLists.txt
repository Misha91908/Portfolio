cmake_minimum_required(VERSION 3.5)
project(untitled1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES main.c)
add_executable(untitled1 ${SOURCE_FILES})