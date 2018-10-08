/* glutils.h - Utilities for using OpenGL
 *
 * Functions and macros for creating and managing OpenGL buffers and shaders
 */
#ifndef GLUTILS_H_INCLUDED
#define GLUTILS_H_INCLUDED

#include <GL/glew.h>
#include <assert.h>
#include <stdint.h>
#include "utils.h"
#include "glutils.h"

/* OpenGL utility functions */
GLuint gen_buffer(GLenum type, GLsizei size, const void *data) ATTR((nonnull(3)));
void   attrib_buffer(GLuint index, GLint size, GLenum type, 
        GLsizei stride, intptr_t offset);
void   del_buffer(GLuint buffer);

GLuint gen_array(void);
void   bind_array(GLuint vao);
void   del_array(GLuint vao);

GLuint make_shader(GLenum type, const char *src)  ATTR((nonnull(2)));
GLuint load_shader(GLenum type, const char *path) ATTR((nonnull(2))); 

GLuint make_program(GLuint shader1, GLuint shader2);
GLuint load_program(const char *vpath, const char *fpath) ATTR((nonnull(1,2)));
void   use_program(GLuint prog); 
void   del_program(GLuint prog);

#if defined(NDEBUG)
# define GLCHECK(call) call
#else
# define GLCHECK(call) do {                            \
    call;                                              \
    GLenum gl_err;                                     \
    bool gl_had_error = false;                         \
    while ((gl_err = glGetError()) != GL_NO_ERROR) {   \
        LOG("%s\nGL error 0x%x: %s",                   \
            #call, gl_err, glEnumName(gl_err));        \
        gl_had_error = true;                           \
    }                                                  \
    if (gl_had_error)                                  \
        exit(EXIT_FAILURE);                            \
} while (0)

const char *glEnumName(GLenum e) ATTR((returns_nonnull, const));

#endif /* NDEBUG */

#endif /* GLUTILS_H_INCLUDED */
