add_executable(main 
    main.c 
    utils.c
    glutils.c
    cube.c
    entity.c
    objloader.c
)
target_link_libraries(main 
    PRIVATE 
        OpenGL::OpenGL 
        GLEW::GLEW 
        SDL2::SDL2main
        SDL2::SDL2_image
        cglm
        klib
)
target_include_directories(main 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
)
target_compile_features(main
    PRIVATE
        c_std_99
        c_static_assert
)
target_compile_options(main
    PRIVATE
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
        # if I hate myself
        # -Wconversion
)
target_compile_definitions(main 
    PRIVATE
        "RESOURCE_DIR=\"${RESOURCE_DIR}\""
        $<$<CONFIG:Release>:"NDEBUG">
)
if (DISABLE_ATTRIBUTES)
    target_compile_definitions(main
        PRIVATE
            "NOATTRIBUTES"
)
endif(DISABLE_ATTRIBUTES)
