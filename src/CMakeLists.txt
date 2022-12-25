add_library(${PROJECT_NAME} STATIC 
    camera.cpp 
    collision.cpp 
    effects.cpp 
    light.cpp 
    objloader.cpp 
    physicsobject.cpp 
    pixelshader.cpp 
    rotation.cpp 
    scene.cpp
    texture.cpp 
    vec.cpp 
    vertex.cpp 
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
