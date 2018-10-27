#ifndef ENTITY_RENDERER_H_INCLUDED
#define ENTITY_RENDERER_H_INCLUDED

#include <GL/glew.h>

struct StaticUniforms {
    GLint transformation_matrix,
          projection_matrix,
          view_matrix;
};

#endif
