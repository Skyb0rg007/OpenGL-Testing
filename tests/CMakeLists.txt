add_executable(asantest asantest.c)
target_link_libraries(asantest
    PRIVATE
        # OpenGL::OpenGL
        # GLEW::GLEW
        ${SDL2_LIBRARIES}
)
target_include_directories(asantest
    PRIVATE
        ${SDL2_INCLUDE_DIRS}
)
target_compile_options(asantest
    PRIVATE
        -Wall -Wextra -pedantic
)
target_compile_features(asantest
    PRIVATE
        c_std_11
)
