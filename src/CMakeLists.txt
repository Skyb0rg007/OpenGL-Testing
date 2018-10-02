add_executable(main main.c shader.c)
target_link_libraries(main 
    PRIVATE 
        OpenGL::OpenGL 
        GLEW::GLEW 
        ${SDL2_LIBRARIES}
)
target_include_directories(main 
    PRIVATE 
        ${SDL2_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
)
target_compile_features(main
    PRIVATE
        c_std_11
)
target_compile_options(main
    PRIVATE
        -Wall -Wextra -pedantic -Wno-dollar-in-identifier-extension
        $<$<CONFIG:Debug>: -fno-omit-frame-pointer -fsanitize=address -g3>
)
target_compile_definitions(main 
    PRIVATE
        "RESOURCE_DIR=\"${RESOURCE_DIR}\""
        $<$<CONFIG:Release>:"NDEBUG">
)
