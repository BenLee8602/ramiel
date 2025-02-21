add_library(${PROJECT_NAME}_graphics STATIC
    camera.cpp
    clip.cpp
    effects.cpp
    entity.cpp
    light.cpp
    line.cpp
    rasterize.cpp
)

target_include_directories(${PROJECT_NAME}_graphics PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_graphics PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_graphics PUBLIC ${PROJECT_NAME}_data)

add_subdirectory(shaderlang)
