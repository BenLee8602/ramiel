add_subdirectory(math)
add_subdirectory(file)
add_subdirectory(data)
add_subdirectory(graphics)
add_subdirectory(physics)

add_library(${PROJECT_NAME} STATIC
    ramiel.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_math)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_file)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_data)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_graphics)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_physics)
