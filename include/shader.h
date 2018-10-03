#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <GL/glew.h>

typedef struct Shader {
    GLuint program;
} Shader;

Shader *Shader_new(const GLchar *vertexPath, const GLchar *fragmentPath)
    __attribute__((nonnull(1, 2), malloc));
Shader *Shader_new_src(const GLchar *vertexSrc, const GLchar *fragmentSrc)
    __attribute__((nonnull(1, 2)));
void Shader_free(Shader *self)
    __attribute__((nonnull(1)));
void Shader_use(const Shader *self)
    __attribute__((nonnull(1)));

#endif /* SHADER_H */
