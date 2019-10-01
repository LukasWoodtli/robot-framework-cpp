project(robot-framework-cpp)

add_library(robot-framework-cpp SHARED robot_framework_cpp.cpp)

install(TARGETS robot-framework-cpp DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../install)
