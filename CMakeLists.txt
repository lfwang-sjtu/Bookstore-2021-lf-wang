cmake_minimum_required(VERSION 3.17)
project(Bookstore_2021)

set(CMAKE_CXX_STANDARD 14)

add_executable(code src/main.cpp src/User.h src/User.cpp src/BlockList.cpp src/BlockList.h)