
include_directories(SYSTEM include/)

add_executable(main_tests main_tests.cpp)
add_test(main_tests main_tests)

add_executable(step_tests step_tests.cpp)
add_test(step_tests step_tests)
