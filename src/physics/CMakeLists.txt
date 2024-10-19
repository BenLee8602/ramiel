add_library(${PROJECT_NAME}_physics STATIC
    collisiondispatch.cpp
    collisionhandle.cpp
)

target_include_directories(${PROJECT_NAME}_physics PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_physics PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_physics PUBLIC ${PROJECT_NAME}_math)
