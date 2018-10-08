project(Structures
    VERSION 1.0
    DESCRIPTION "Data structures as implemented by the Linux Kernel"
    LANGUAGES C
)

add_library(structures INTERFACE)
target_include_directories(structures
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_compile_features(structures
    INTERFACE
        c_std_11
)
