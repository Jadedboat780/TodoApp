
cmake_minimum_required(VERSION 3.16)

project(ToDoApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ToDoApp
    src/main.cpp
    src/todo.cpp
    src/todo.h
    )

target_link_libraries(ToDoApp PRIVATE Qt6::Widgets)


