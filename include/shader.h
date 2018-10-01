
#pragma once

#include <stdio.h>
#include <GL/glew.h>

typedef struct Shader {
    GLuint program;
} Shader;

Shader *Shader_new(const GLchar *vertexPath, const GLchar *fragmentPath);
Shader *Shader_new_src(const GLchar *vertexSrc, const GLchar *fragmentSrc);
void Shader_free(Shader *self);
void Shader_use(const Shader *self);
