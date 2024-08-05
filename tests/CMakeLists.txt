add_executable("${PROJECT_NAME}_test" 
    framework/main.cpp 
    framework/test.cpp 
    tests/data/texture.test.cpp 
    tests/data/tuple.test.cpp 
    tests/data/voidbuffer.test.cpp 
    tests/file/image.test.cpp 
    tests/file/objloader.test.cpp 
    tests/graphics/camera.test.cpp 
    tests/graphics/effects.test.cpp 
    tests/graphics/light.test.cpp 
    tests/graphics/vertex.test.cpp 
    tests/math/mat.test.cpp 
    tests/math/transform.test.cpp 
    tests/math/vec.test.cpp 
    tests/physics/collisionhandle.test.cpp 
    tests/physics/collisiondispatch.test.cpp 
)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_SOURCE_DIR}/tests/framework/include)
target_compile_definitions(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/data")
target_link_libraries(${PROJECT_NAME}_test $<TARGET_FILE:${PROJECT_NAME}>)
