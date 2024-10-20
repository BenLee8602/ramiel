add_library(${PROJECT_NAME}_testframework STATIC test.cpp)
target_include_directories(${PROJECT_NAME}_testframework PUBLIC include)
