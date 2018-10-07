add_executable(asantest EXCLUDE_FROM_ALL asantest.c)
target_link_libraries(asantest
    PRIVATE
        # OpenGL::OpenGL
        # GLEW::GLEW
        SDL2::SDL2
)
target_compile_options(asantest
    PRIVATE
        -Wall -Wextra -pedantic
)
target_compile_features(asantest
    PRIVATE
        c_std_11
)
