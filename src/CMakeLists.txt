add_subdirectory(math)
add_subdirectory(file)
add_subdirectory(data)
add_subdirectory(graphics)
add_subdirectory(physics)

# engine is windows only for now
if(WIN32)
    add_subdirectory(engine)
endif()
