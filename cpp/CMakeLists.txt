project(robot-framework-cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(robot-framework-cpp SHARED robot_framework_cpp.cpp)

install(TARGETS robot-framework-cpp DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../install)
