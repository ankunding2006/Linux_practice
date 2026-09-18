cmake_minimum_required(VERSION 3.10)

project(pthreaed LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(pthreaed pthreaed.cpp)
target_link_libraries(pthreaed PRIVATE Threads::Threads)