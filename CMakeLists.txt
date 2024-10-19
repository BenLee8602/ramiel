cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
project(ramiel)

option(${PROJECT_NAME}_BUILD_EXAMPLE "build examples" ON)
option(${PROJECT_NAME}_BUILD_TESTS "build tests" ON)

add_subdirectory(src)

if(${PROJECT_NAME}_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(tests)
endif()
