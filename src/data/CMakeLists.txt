add_library(${PROJECT_NAME}_data STATIC
    json.cpp
    mesh.cpp
    texture.cpp
)

target_include_directories(${PROJECT_NAME}_data PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_data PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_data PUBLIC ${PROJECT_NAME}_file)
