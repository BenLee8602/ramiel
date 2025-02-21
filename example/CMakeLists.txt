add_subdirectory(shaders)

add_executable(${PROJECT_NAME}_example main.cpp)

target_include_directories(${PROJECT_NAME}_example PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}_example PRIVATE ${CMAKE_SOURCE_DIR}/example)
target_include_directories(${PROJECT_NAME}_example PRIVATE ${CMAKE_BINARY_DIR}/example/shaders)

target_link_libraries(${PROJECT_NAME}_example PUBLIC gdi32)
target_link_libraries(${PROJECT_NAME}_example PUBLIC ${PROJECT_NAME}_example_shaders)
