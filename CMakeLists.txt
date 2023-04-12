cmake_minimum_required(VERSION 3.18)
project(todo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(todo main.cpp TodoApp.h colors.h json.h)
