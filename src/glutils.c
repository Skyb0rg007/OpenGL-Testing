#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "utils.h"
#include "glutils.h"

static GLchar *load_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        FATAL("Could not open %s: %s", path, strerror(errno));
    }

    fseek(file, 0, SEEK_END);
    int file_length = ftell(file);
    rewind(file);
    char *data = calloc(file_length + 1, sizeof (char));
    fread(data, sizeof (char), file_length, file);
    fclose(file);
    return data;
}

GLuint gen_buffer(GLenum type, GLsizei size, const void *data)
{
    GLuint buffer;
    GLCHECK(glGenBuffers(1, &buffer));
    GLCHECK(glBindBuffer(type, buffer));
    GLCHECK(glBufferData(type, size, data, GL_STATIC_DRAW));
    return buffer;
}

void attrib_buffer(GLuint idx, GLint size, GLenum type, GLsizei stride, intptr_t offset)
{
    static_assert(sizeof (intptr_t) == sizeof (GLvoid *), 
            "sizeof(intptr_t) != sizeof(GLvoid *)\n"
            "offset argument is wrong precision");
    /* Hard code buffer to not be normalized */
    /* Note: use NULL when offset is 0 since sometimes (int)NULL != (int)0 */
    GLCHECK(glVertexAttribPointer(idx, size, type, GL_FALSE, stride, 
                offset == 0 ? NULL : (const GLvoid *)offset));
    GLCHECK(glEnableVertexAttribArray(idx));
}

void del_buffer(GLuint buffer)
{
    GLCHECK(glDeleteBuffers(1, &buffer));
}

GLuint gen_array(void)
{
    GLuint vao;
    GLCHECK(glGenVertexArrays(1, &vao));
    bind_array(vao);
    return vao;
}

void bind_array(GLuint vao)
{
    GLCHECK(glBindVertexArray(vao));
} 

void del_array(GLuint vao)
{
    GLCHECK(glDeleteVertexArrays(1, &vao));
}

GLuint make_shader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    GLCHECK(glShaderSource(shader, 1, &source, NULL));
    GLCHECK(glCompileShader(shader));

    /* Error checking */
    GLint status;
    GLCHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE) {
        GLint status_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status_length);
        GLchar *info = calloc(status_length, sizeof (GLchar));
        glGetShaderInfoLog(shader, status_length, NULL, info);
        FATAL("Error compiling shader: %s", info);
        free(info);
    }
    return shader;
}

GLuint load_shader(GLenum type, const char *path)
{
    GLchar *data = load_file(path);
    GLuint shader = make_shader(type, data);
    free(data);
    return shader;
}

GLuint make_program(GLuint shader1, GLuint shader2)
{
    GLuint program = glCreateProgram();
    GLCHECK(glAttachShader(program, shader1));
    GLCHECK(glAttachShader(program, shader2));
    GLCHECK(glLinkProgram(program));

    /* Error checking */
    GLint status;
    GLCHECK(glGetProgramiv(program, GL_LINK_STATUS, &status));
    if (status == GL_FALSE) {
        GLint status_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &status_length);
        GLchar *info = calloc(status_length, sizeof (GLchar));
        glGetProgramInfoLog(program, status_length, NULL, info);
        FATAL("Error linking program: %s", info);
        free(info);
    }

    GLCHECK(glDetachShader(program, shader1));
    GLCHECK(glDetachShader(program, shader2));
    GLCHECK(glDeleteShader(shader1));
    GLCHECK(glDeleteShader(shader2));
    return program;
}

GLuint load_program(const char *vertexpath, const char *fragmentpath)
{
    GLuint vertex   = load_shader(GL_VERTEX_SHADER, vertexpath);
    GLuint fragment = load_shader(GL_FRAGMENT_SHADER, fragmentpath);
    return make_program(vertex, fragment);
}

void use_program(GLuint prog)
{
    GLCHECK(glUseProgram(prog));
}

void del_program(GLuint prog)
{
    GLCHECK(glDeleteProgram(prog));
}

#ifndef NDEBUG
const char *glEnumName(GLenum e)
{
    switch(e)
    {
    case GL_TEXTURE:
        return "GL_TEXTURE";
    case GL_RENDERBUFFER:
        return "GL_RENDERBUFFER";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case GL_FRAMEBUFFER_UNSUPPORTED:
        return "GL_FRAMEBUFFER_UNSUPPORTED";
    default:
        return "(unknown)";
    }
}
#endif /* ! NDEBUG */

