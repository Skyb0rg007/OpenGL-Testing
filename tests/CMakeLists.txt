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

add_executable(logtest EXCLUDE_FROM_ALL test.c)
target_link_libraries(logtest
    PRIVATE
        SDL2::SDL2
        # SQLite3::SQLite3
)
target_compile_options(logtest
    PRIVATE
        # -Wall -Wextra -pedantic
        $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:
            -Wall -Wextra -pedantic
            -Wformat=2 -Wwrite-strings -Wstrict-overflow=5
            -Winline -Wnested-externs 
            -Wno-double-promotion
            -Wshadow -Wformat=2
            -fno-common -Wfloat-equal 
        >
        $<$<C_COMPILER_ID:GNU>:
            -Wlogical-op -Wstrict-aliasing=2
            -Wjump-misses-init -Wcast-align=strict 
        >
        $<$<C_COMPILER_ID:Clang>:
            -Wcast-align 
        >
)
target_compile_features(logtest
    PRIVATE
        c_std_99
)

add_executable(structtest EXCLUDE_FROM_ALL structtest.c)
target_link_libraries(structtest
    PRIVATE
        structures
)
target_compile_options(structtest
    PRIVATE
        -Wall -Wextra -pedantic
)
