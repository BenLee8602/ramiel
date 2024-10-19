add_library(${PROJECT_NAME} STATIC 
    graphics/camera.cpp 
    graphics/clip.cpp 
    physics/collisionhandle.cpp 
    physics/collisiondispatch.cpp 
    graphics/effects.cpp 
    file/image.cpp 
    math/interpolate.cpp 
    graphics/light.cpp 
    file/objloader.cpp 
    ramiel.cpp 
    graphics/rasterize.cpp 
    data/texture.cpp 
    data/voidbuffer.cpp 
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_deps_stb)
