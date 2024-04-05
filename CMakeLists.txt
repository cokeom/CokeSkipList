cmake_minimum_required(VERSION 3.27)
project(MySkipList)

set(CMAKE_CXX_STANDARD 17)

add_executable(MySkipList main.cpp
        CokeSkipList.h
        Node.h)

add_executable(StressTest StressTest.cpp CokeSkipList.h Node.h)