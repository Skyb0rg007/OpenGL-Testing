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


# Load all the needed packages
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL 3.3 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW 2.1 REQUIRED)
find_package(GSL 2.0 REQUIRED)

message("GLEW include path: ${GLEW_INCLUDE_DIR}")

# Enable Address Sanitizer for building executables
# (cmake 3.13 better have target_linker_options)
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG 
    " -fno-omit-frame-pointer -fsanitize=address ")

# Compile libraries
add_subdirectory(libs/algebra)

# Location for shaders
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources/")

add_subdirectory(src)

# enable_testing()
add_subdirectory(tests)
