add_executable(${PROJECT_NAME}_engine
    command.cpp
    engine.cpp
    entity.cpp
    event.cpp
    graphics.cpp
    main.cpp
    task.cpp
    window.cpp
)

target_include_directories(${PROJECT_NAME}_engine PUBLIC ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME}_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_engine PUBLIC ${PROJECT_NAME}_graphics)
target_link_libraries(${PROJECT_NAME}_engine PUBLIC ${PROJECT_NAME}_physics)
