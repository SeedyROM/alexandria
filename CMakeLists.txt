cmake_minimum_required(VERSION 3.21)
project(babel C)

set(CMAKE_C_STANDARD 11)
include_directories(src/)

add_executable(babel src/main.c src/wal.c src/memtable.c)
