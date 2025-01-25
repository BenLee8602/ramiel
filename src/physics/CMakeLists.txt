add_library(${PROJECT_NAME}_physics STATIC
    broadcollision.cpp
    collision.cpp
    constraint.cpp
    entity.cpp
    inertia.cpp
    physics.cpp
    softbody.cpp
)

target_include_directories(${PROJECT_NAME}_physics PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_physics PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_physics PUBLIC ${PROJECT_NAME}_math)
