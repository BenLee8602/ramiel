cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
project(ramiel)

option(${PROJECT_NAME}_BUILD_TESTS "build tests" ON)

add_subdirectory(deps)
add_subdirectory(src)

if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(tests)
endif()
