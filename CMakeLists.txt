cmake_minimum_required(VERSION 3.3)
project(BPlusTree)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(BPlusTree ${SOURCE_FILES})