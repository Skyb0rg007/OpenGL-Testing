project(klib
    DESCRIPTION "Klib is a standalone and lightweight C library"
    HOMEPAGE_URL "https://github.com/attractivechaos/klib"
    LANGUAGES C
)

add_library(klib INTERFACE)
target_include_directories(klib
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_compile_features(klib
    INTERFACE
        c_std_90
)
