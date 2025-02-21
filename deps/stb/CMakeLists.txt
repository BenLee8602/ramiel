add_library(${PROJECT_NAME}_deps_stb STATIC stb_image.cpp)
target_include_directories(${PROJECT_NAME}_deps_stb PUBLIC ${CMAKE_SOURCE_DIR}/deps)
