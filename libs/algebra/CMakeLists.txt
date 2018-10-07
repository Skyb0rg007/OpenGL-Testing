add_library(algebra algebra.c)
target_link_libraries(algebra
    PUBLIC
        GSL::gsl
)
target_include_directories(algebra
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(algebra
    PRIVATE
        c_std_11
)
target_compile_options(algebra
    PRIVATE
        -Wall -Wextra -pedantic -Wimplicit-fallthrough
        $<$<CONFIG:Debug>: -fno-omit-frame-pointer -fsanitize=address -g3>
)
target_compile_definitions(algebra
    PRIVATE
        $<$<CONFIG:Release>:"NDEBUG">
)
