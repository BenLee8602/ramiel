add_library(${PROJECT_NAME}_tests_physics OBJECT
    physics.test.cpp
)

target_include_directories(${PROJECT_NAME}_tests_physics PRIVATE ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_tests_physics PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}_tests_physics PRIVATE ${CMAKE_SOURCE_DIR}/tests/framework/include)

target_compile_definitions(${PROJECT_NAME}_tests_physics PRIVATE ${PROJECT_NAME}_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/data")

target_link_libraries(${PROJECT_NAME}_tests_physics PUBLIC ${PROJECT_NAME}_physics)
