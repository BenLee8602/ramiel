add_executable(${PROJECT_NAME}_engine
    menu/terminal.cpp
    menu/camera.cpp
    command.cpp
    entity.cpp
    event.cpp
    graphics.cpp
    main.cpp
    menu.cpp
    physics.cpp
    shader.cpp
    task.cpp
    window.cpp
)

target_include_directories(
    ${PROJECT_NAME}_engine
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/engine
)

target_include_directories(${PROJECT_NAME}_engine PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_definitions(${PROJECT_NAME}_engine PRIVATE ${PROJECT_NAME}_ENGINE_ASSET_DIR="${CMAKE_SOURCE_DIR}/src/engine/assets")

target_link_libraries(${PROJECT_NAME}_engine PUBLIC ${PROJECT_NAME}_graphics)
target_link_libraries(${PROJECT_NAME}_engine PUBLIC ${PROJECT_NAME}_physics)

target_include_directories(${PROJECT_NAME}_engine PRIVATE ${CMAKE_SOURCE_DIR}/deps/sdl/include)
target_link_libraries(${PROJECT_NAME}_engine PRIVATE SDL3::SDL3)
add_custom_command(
    TARGET ${PROJECT_NAME}_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:SDL3::SDL3>"
    "$<TARGET_FILE_DIR:ramiel_engine>"
)

