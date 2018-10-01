cmake_minimum_required(VERSION 3.1)
project(GL_testing)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL 3.3 REQUIRED) # Only need 3.3 for modern shader stuff
find_package(SDL2 REQUIRED) # cannot do versioning since no imported targets
find_package(GLEW 2.1 REQUIRED) # most recent version

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources/")
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
