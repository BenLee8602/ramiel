add_library(${PROJECT_NAME} STATIC 
    graphics/camera.cpp 
    physics/collisionhandle.cpp 
    physics/collisiondispatch.cpp 
    graphics/effects.cpp 
    graphics/graphics.cpp 
    file/image.cpp 
    graphics/light.cpp 
    file/objloader.cpp 
    ramiel.cpp 
    data/texture.cpp 
    graphics/vertex.cpp 
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
