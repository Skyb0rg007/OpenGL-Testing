cmake_minimum_required(VERSION 3.1)
project(GL_testing
    VERSION 0.1.0
    DESCRIPTION "Testing OpenGL and SDL2"
    HOMEPAGE_URL "https://github.com/Skyb0rg007/OpenGL-Testing"
    LANGUAGES C
)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL 3.3 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW 2.1 REQUIRED)

# Enable Address Sanitizer for debug builds
# (cmake 3.13 better have target_linker_flags)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("Debug build type selected")
    set(CMAKE_C_FLAGS_DEBUG 
        "${CMAKE_C_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG 
        "${CMAKE_LINKER_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=address")
endif()

# Location for shaders
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources/")
add_subdirectory(src)

# enable_testing()
add_subdirectory(tests)
