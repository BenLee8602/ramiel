add_executable("${PROJECT_NAME}_test" 
    ${CMAKE_SOURCE_DIR}/deps/catch2/main.cpp 
    graphics/camera.test.cpp 
    physics/collision.test.cpp 
    physics/dynamics.test.cpp 
    graphics/effects.test.cpp 
    graphics/light.test.cpp 
    math/mat.test.cpp 
    file/objloader.test.cpp 
    math/rotation.test.cpp 
    graphics/texture.test.cpp 
    math/transform.test.cpp 
    math/vec.test.cpp
)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/test_data")
target_link_libraries(${PROJECT_NAME}_test $<TARGET_FILE:${PROJECT_NAME}>)
