add_library(${PROJECT_NAME}_graphics_core OBJECT
    camera.cpp
    clip.cpp
    effects.cpp
    entity.cpp
    light.cpp
    line.cpp
    rasterize.cpp
)

target_include_directories(${PROJECT_NAME}_graphics_core PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_graphics_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_graphics_core PUBLIC ${PROJECT_NAME}_data)

add_subdirectory(shaderlang)
add_subdirectory(shaders)

add_library(${PROJECT_NAME}_graphics STATIC)
target_link_libraries(${PROJECT_NAME}_graphics PUBLIC ${PROJECT_NAME}_graphics_core)
target_link_libraries(${PROJECT_NAME}_graphics PUBLIC ${PROJECT_NAME}_graphics_shaders)
