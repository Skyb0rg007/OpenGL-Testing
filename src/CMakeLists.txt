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
        -Wall -Wextra -pedantic
        -Wlogical-op -Wrestrict -Wnull-dereference
        -Winline -Wvla
        -Wjump-misses-init -Wno-double-promotion
        -Wshadow -Wformat=2
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
