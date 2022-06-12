add_library(${PROJECT_NAME} STATIC 
    camera.cpp camera.h 
    collision.cpp collision.h 
    draw.cpp draw.h 
    effects.cpp effects.h 
    entity.cpp entity.h 
    light.cpp light.h 
    model.cpp model.h 
    objloader.cpp objloader.h 
    physics.cpp physics.h 
    physicsobj.cpp physicsobj.h 
    ${CMAKE_SOURCE_DIR}/include/ramiel.h 
    graphics.cpp graphics.h 
    texture.cpp texture.h 
    triangle.h 
    vec.cpp vec.h
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
