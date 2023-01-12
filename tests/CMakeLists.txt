add_executable("${PROJECT_NAME}_test" 
    catch2/main.cpp 
    camera.test.cpp 
    collision.test.cpp 
    dynamics.test.cpp 
    effects.test.cpp 
    light.test.cpp 
    texture.test.cpp 
    transform.test.cpp 
    vec.test.cpp
)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/data")
target_link_libraries(${PROJECT_NAME}_test $<TARGET_FILE:${PROJECT_NAME}>)
