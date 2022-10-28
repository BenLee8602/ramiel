add_library(${PROJECT_NAME} STATIC 
    ${CMAKE_SOURCE_DIR}/include/ramiel/ramiel.h 
    ${CMAKE_SOURCE_DIR}/include/ramiel/args.h
    camera.cpp camera.h 
    collision.cpp collision.h 
    draw.cpp draw.h 
    effects.cpp effects.h 
    entity.cpp entity.h 
    graphics.cpp graphics.h 
    light.cpp light.h 
    mesh.cpp mesh.h 
    objloader.cpp objloader.h 
    physics.cpp physics.h 
    physicsobj.cpp physicsobj.h 
    pixelshader.cpp pixelshader.h 
    rotation.cpp rotation.h 
    texture.cpp texture.h 
    triangle.h 
    vec.cpp vec.h 
    vertex.cpp vertex.h 
    vertexshader.cpp vertexshader.h
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
