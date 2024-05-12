add_library(${PROJECT_NAME} STATIC 
    graphics/camera.cpp 
    physics/collisionhandle.cpp 
    physics/collisiondispatch.cpp 
    graphics/effects.cpp 
    file/image.cpp 
    graphics/light.cpp 
    file/objloader.cpp 
    ramiel.cpp 
    graphics/texture.cpp 
    graphics/vertex.cpp 
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
