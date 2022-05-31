add_executable("${PROJECT_NAME}_test" 
    catch2/main.cpp
    camera.test.cpp 
    light.test.cpp 
    model.test.cpp 
    objloader.test.cpp 
    physics.test.cpp 
    texture.test.cpp 
    vec.test.cpp
)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/data")
target_link_libraries(${PROJECT_NAME}_test $<TARGET_FILE:${PROJECT_NAME}>)
