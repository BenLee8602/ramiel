add_executable(${PROJECT_NAME}_example main.cpp)

target_include_directories(${PROJECT_NAME}_example PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}_example PRIVATE ${CMAKE_SOURCE_DIR}/example)

target_link_libraries(${PROJECT_NAME}_example gdi32)
target_link_libraries(${PROJECT_NAME}_example $<TARGET_FILE:${PROJECT_NAME}>)
