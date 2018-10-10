#include <GL/glew.h>
#include <cglm/cglm.h>
#include "cube.h"
#include "entity.h"
#include "glutils.h"

/* Constants */
static const char *const VERTEX_FILE   = RESOURCE_DIR "cube.vertex.glsl";
static const char *const FRAGMENT_FILE = RESOURCE_DIR "cube.fragment.glsl";
static const char *const TEXTURE_FILE  = RESOURCE_DIR "justinian.png";
static const GLfloat vertices[3*24] = {
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f
};
static const GLfloat texture_coords[2*24] = {
    0, 0,
    0, 1,
    1, 1,
    1, 0,
    0, 0,
    0, 1,
    1, 1,
    1, 0,
    0, 0,
    0, 1,
    1, 1,
    1, 0,
    0, 0,
    0, 1,
    1, 1,
    1, 0,
    0, 0,
    0, 1,
    1, 1,
    1, 0,
    0, 0,
    0, 1,
    1, 1,
    1, 0
};
static const GLuint indices[3*12] = {
     0,  1,  3,
     3,  1,  2,
     4,  5,  7,
     7,  5,  6,
     8,  9, 11,
    11,  9, 10,
    12, 13, 15,
    15, 13, 14,
    16, 17, 19,
    19, 17, 18,
    20, 21, 23,
    23, 21, 22
};
static const GLfloat normals[] = {
    0.0f
};

/* Globals */
static struct Model g_model;

void init_cube(void)
{
    static const struct ModelData data = {
        .vert_filepath = VERTEX_FILE,
        .frag_filepath = FRAGMENT_FILE,
        .tex_filepath  = TEXTURE_FILE,
        .vertices = vertices,
        .vertices_count = ARRAY_SIZE(vertices),
        .texture_uv = texture_coords,
        .texture_uv_count = ARRAY_SIZE(texture_coords),
        .normals = normals,
        .normals_count = 0,
        .indices = indices,
        .indices_count = ARRAY_SIZE(indices)
    };
    create_model(&data, &g_model);
}

void cleanup_cube(void)
{
    destroy_model(&g_model);
}

void render_cube(const struct Entity *entity)
{
    render_entity(&g_model, entity);
}
