project(robot-framework-cpp)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(unit-tests)

add_library(robot-framework-cpp SHARED robot_framework_cpp.cpp)

install(TARGETS robot-framework-cpp DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../install)
