add_subdirectory(framework)
add_subdirectory(tests)

add_executable(${PROJECT_NAME}_tests main.cpp)

target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME}_testframework)

target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME}_tests_data)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME}_tests_engine)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME}_tests_file)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME}_tests_graphics)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME}_tests_math)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME}_tests_physics)
