add_library(${PROJECT_NAME}_engine STATIC
    engine.cpp
    entity.cpp
    event.cpp
    graphics.cpp
)

target_include_directories(${PROJECT_NAME}_engine PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_engine PUBLIC ${PROJECT_NAME}_graphics)
target_link_libraries(${PROJECT_NAME}_engine PUBLIC ${PROJECT_NAME}_physics)
