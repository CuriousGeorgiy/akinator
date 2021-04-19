cmake_minimum_required(VERSION 3.17)
project(akinator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(akinator)

target_sources(akinator PUBLIC Main.cpp Akinator.cpp BinaryTree.cpp File.cpp String.cpp Canary.cpp)
target_include_directories(akinator PUBLIC ${PROJECT_SOURCE_DIR})
