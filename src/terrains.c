#include <stdlib.h>
#include "terrains.h"
#include "entity.h"

static const float SIZE = 800;
static const int VERTEX_COUNT = 128;

void generate_terrain(struct ModelData *out)
{
    static const int count = VERTEX_COUNT * VERTEX_COUNT;

    float *vertices   = calloc(count * 3, sizeof (float));
    float *normals    = calloc(count * 3, sizeof (float));
    float *texture_uv = calloc(count * 2, sizeof (float));
    unsigned *indices = calloc(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1), 
            sizeof (unsigned));

    unsigned vertex_pointer = 0;

    for (unsigned i = 0; i < VERTEX_COUNT; i++) {
        for (unsigned j = 0; j < VERTEX_COUNT; j++) {
            vertices[vertex_pointer*3]   = (float)j / (VERTEX_COUNT - 1) * SIZE;
            vertices[vertex_pointer*3+1] = 0;
            vertices[vertex_pointer*3+2] = (float)i / (VERTEX_COUNT - 1) * SIZE;

            normals[vertex_pointer*3]   = 0;
            normals[vertex_pointer*3+1] = 1;
            normals[vertex_pointer*3+2] = 0;

            texture_uv[vertex_pointer*2]   = (float)j / (VERTEX_COUNT - 1);
            texture_uv[vertex_pointer*2+1] = (float)i / (VERTEX_COUNT - 1);
            vertex_pointer++;
        }
    }

    unsigned pointer = 0;

    for (unsigned gz = 0; gz < VERTEX_COUNT - 1; gz++) {
        for (unsigned gx = 0; gx < VERTEX_COUNT - 1; gx++) {
            unsigned top_left = (gz * VERTEX_COUNT) + gx;
            unsigned top_right = top_left + 1;
            unsigned bottom_left = ((gz + 1) * VERTEX_COUNT) + gx;
            unsigned bottom_right = bottom_left + 1;

            indices[pointer++] = top_left;
            indices[pointer++] = bottom_left;
            indices[pointer++] = top_right;
            indices[pointer++] = top_right;
            indices[pointer++] = bottom_left;
            indices[pointer++] = bottom_right;
        }
    }

    out->vertices_count = count * 3;
    out->texture_uv_count = count * 2;
    out->normals_count = count * 3;
    out->indices_count = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1);

    out->vertices = vertices;
    out->texture_uv = texture_uv;
    out->normals = normals;
    out->indices = indices;
}

void free_terrain_modeldata(struct ModelData *m)
{
    if (m != NULL) {
        free((void *)m->vertices);
        free((void *)m->texture_uv);
        free((void *)m->normals);
        free((void *)m->indices);
    }
}

void load_terrain_model(const char *texturefile, const char *vertexfile,
        const char *fragmentfile, struct Model *m)
{
    struct ModelData data = {
        .vert_filepath = vertexfile,
        .frag_filepath = fragmentfile,
        .tex_filepath  = texturefile
    };
    generate_terrain(&data);
    create_model(&data, m);
    free_terrain_modeldata(&data);
}
