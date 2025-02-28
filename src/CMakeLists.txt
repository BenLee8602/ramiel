add_subdirectory(math)
add_subdirectory(file)
add_subdirectory(data)
add_subdirectory(graphics)
add_subdirectory(physics)
add_subdirectory(engine)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}_math)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}_file)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}_data)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}_graphics)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}_physics)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}_engine)
