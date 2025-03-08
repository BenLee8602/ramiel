function(build_shader shader_name)
    add_custom_command(
        OUTPUT ${shader_name}.h ${shader_name}.cpp
        COMMAND ${PROJECT_NAME}_shaderlang ${CMAKE_CURRENT_LIST_DIR}/${shader_name} .
        DEPENDS ${PROJECT_NAME}_graphics_core ${PROJECT_NAME}_shaderlang ${CMAKE_CURRENT_LIST_DIR}/${shader_name}
    )
endfunction()

build_shader(VertexShader)
build_shader(VertexShaderTextured)

build_shader(PixelShader)
build_shader(PixelShaderTextured)

add_library(${PROJECT_NAME}_graphics_shaders OBJECT
    VertexShader.cpp
    VertexShaderTextured.cpp

    PixelShader.cpp
    PixelShaderTextured.cpp
)

target_include_directories(${PROJECT_NAME}_graphics_shaders PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}_graphics_shaders PUBLIC ${CMAKE_BINARY_DIR}/src/graphics/shaders)
