cmake_minimum_required(VERSION 3.0.0)
project(Problem VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Problem main.cpp)
add_executable(Problem_v2 main_v2.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
