#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <GL/glew.h>
#include "shader.h"
#include "utils.h"

static GLchar *read_file(const char *path) __attribute__((nonnull(1)));
static long file_size(FILE *) __attribute__((nonnull(1)));

Shader *Shader_new_src(const GLchar *vertexSrc, const GLchar *fragmentSrc)
{
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSrc, NULL);
    glCompileShader(vertex);

    GLint success;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infolog[512];
        glGetShaderInfoLog(vertex, sizeof infolog, NULL, infolog);
        $fatal("Error compiling vertex shader: %s", infolog);
    }

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSrc, NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infolog[512];
        glGetShaderInfoLog(fragment, sizeof infolog, NULL, infolog);
        $fatal("Error compiling fragment shader: %s", infolog);
    }

    Shader *self = malloc(sizeof *self);
    if (self == NULL)
        $fatal("Out of memory");

    // Create shader program
    self->program = glCreateProgram();
    glAttachShader(self->program, vertex);
    glAttachShader(self->program, fragment);
    glLinkProgram(self->program);

    glGetProgramiv(self->program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infolog[512];
        glGetProgramInfoLog(self->program, sizeof infolog, NULL, infolog);
        $fatal("Error linking shader program: %s", infolog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return self;
}

Shader *Shader_new(const GLchar *vertexPath, const GLchar *fragmentPath)
{
    // Create shaders from the filenames
    GLchar *vShaderCode = read_file(vertexPath);
    GLchar *fShaderCode = read_file(fragmentPath);

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar **)&vShaderCode, NULL);
    glCompileShader(vertex);

    GLint success;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infolog[512];
        glGetShaderInfoLog(vertex, sizeof infolog, NULL, infolog);
        $fatal("Error compiling vertex shader: %s", infolog);
    }

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar **)&fShaderCode, NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infolog[512];
        glGetShaderInfoLog(fragment, sizeof infolog, NULL, infolog);
        $fatal("Error compiling fragment shader: %s", infolog);
    }

    Shader *self = malloc(sizeof *self);
    if (self == NULL)
        $fatal("Out of memory");

    // Create shader program
    self->program = glCreateProgram();
    glAttachShader(self->program, vertex);
    glAttachShader(self->program, fragment);
    glLinkProgram(self->program);

    glGetProgramiv(self->program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infolog[512];
        glGetProgramInfoLog(self->program, sizeof infolog, NULL, infolog);
        $fatal("Error linking shader program: %s", infolog);
    }

    free(vShaderCode);
    free(fShaderCode);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return self;
}

void Shader_free(Shader *self)
{
    glDeleteProgram(self->program);
    free(self);
}

void Shader_use(const Shader *self)
{
    glUseProgram(self->program);
}

static GLchar *read_file(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL)
        $fatal("Could not open %s: %s", path, strerror(errno));

    const long f_len = file_size(f);

    GLchar *buf = malloc(f_len+1);
    if (buf == NULL)
        $fatal("Out of memory");

    fread(buf, 1, f_len, f);
    if (ferror(f))
        $fatal("Error reading file %s", path);
    else if (feof(f))
        $fatal("Reached EOF before reading %ld bytes of %s", f_len, path);

    int ret = fclose(f);
    if (ret == EOF)
        $fatal("Error closing %s: %s", path, strerror(errno));

    buf[f_len] = '\0';
    return buf;
}

static long file_size(FILE *file)
{
    int ret = fseek(file, 0L, SEEK_END);
    if (ret != 0)
        $fatal("Error calling fseek(): %s", strerror(errno));

    long len = ftell(file);
    if (len < 0)
        $fatal("Error calling ftell(): %s", strerror(errno));

    rewind(file);
    return len;
}
