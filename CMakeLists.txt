cmake_minimum_required(VERSION 3.13)
project(ParallelP_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        ParallelP_1 main.cpp
        MatrixSumThread.cpp
        MatrixSumThread.h)