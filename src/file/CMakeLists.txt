add_library(${PROJECT_NAME}_file STATIC
    image.cpp
    objloader.cpp
)

target_include_directories(${PROJECT_NAME}_file PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_file PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_file PUBLIC ${PROJECT_NAME}_math)
target_link_libraries(${PROJECT_NAME}_file PRIVATE ${PROJECT_NAME}_deps_stb)
