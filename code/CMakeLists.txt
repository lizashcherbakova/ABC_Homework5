cmake_minimum_required(VERSION 3.5)

project(abc5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(abc5 main.cpp threadswork.cpp)

target_link_libraries(abc5 pthread)
