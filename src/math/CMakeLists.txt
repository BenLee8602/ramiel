add_library(${PROJECT_NAME}_math STATIC
    curve.cpp
    interpolate.cpp
    quaternion.cpp
)

target_include_directories(${PROJECT_NAME}_math PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_math PUBLIC ${CMAKE_SOURCE_DIR}/include)
