#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <GL/glew.h>
#include "utils.h"

struct Entity {
    float x, y, z;
    float rot_x, rot_y, rot_z;
    float scale;
};

struct Model {
    GLuint program,
           vao,
           vbo_vertices,
           vbo_texture_uv,
           vbo_normals,
           ebo_indices,
           texture;
    GLint matrix_uniform;
    GLsizei num_indices;
};

struct ModelData {
    const char *vert_filepath;
    const char *frag_filepath;
    const char *tex_filepath;
    const GLfloat *vertices;
    GLsizei vertices_count;
    const GLfloat *texture_uv;
    GLsizei texture_uv_count;
    const GLfloat *normals;
    GLsizei normals_count;
    const GLuint *indices;
    GLsizei indices_count;
};

void create_model(const struct ModelData *data, struct Model *out) 
    ATTR((nonnull(1,2)));
void destroy_model(const struct Model *model) 
    ATTR((nonnull(1)));

void render_entity(const struct Model *model, const struct Entity *entity)
    ATTR((nonnull(1, 2)));
void render_entities(const struct Model *model, const struct Entity *entity, 
        size_t nentities) 
    ATTR((nonnull(1, 2)));

#endif
