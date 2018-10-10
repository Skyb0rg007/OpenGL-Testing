#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "glutils.h"
#include "vector.h"
#include "objloader.h"

void load_obj(const char *filepath, struct ModelData *out)
{
    struct FloatVector vertices   = VEC_INIT(vertices, 64);

    struct FloatVector normals    = VEC_INIT(normals, 64);

    struct FloatVector texture_uv = VEC_INIT(texture_uv, 64);

    FILE *file = fopen(filepath, "rt");
    char *line = NULL;
    size_t len = 0;

    /* Load vertices, texture-coords, and normals */
    while (my_getline(&line, &len, file) > 0) 
    {
        if (strncmp(line, "v ", 2) == 0) {
            CHECK_SIZE(vertices, 3);
            VEC_NEXT(vertices) = tofloat(strtok(line+2, " "));
            VEC_NEXT(vertices) = tofloat(strtok(NULL, " "));
            VEC_NEXT(vertices) = tofloat(strtok(NULL, " "));
        } else if (strncmp(line, "vt ", 3) == 0) {
            CHECK_SIZE(texture_uv, 2);
            VEC_NEXT(texture_uv) = tofloat(strtok(line+3, " "));
            VEC_NEXT(texture_uv) = tofloat(strtok(NULL, " "));
        } else if (strncmp(line, "vn ", 3) == 0) {
            CHECK_SIZE(normals, 3);
            VEC_NEXT(normals) = tofloat(strtok(line+3, " "));
            VEC_NEXT(normals) = tofloat(strtok(NULL, " "));
            VEC_NEXT(normals) = tofloat(strtok(NULL, " "));
        } else if (strncmp(line, "f ", 2) == 0) {
            break;
        }
    }

    struct UIntVector indices         = VEC_INIT(indices, 64);

    float *normals_out    = calloc(vertices.size, sizeof (float));
    float *texture_uv_out = calloc(vertices.size, sizeof (float));

    do {
        if (strncmp(line, "f ", 2) == 0) {
            CHECK_SIZE(indices, 3);
            strtok(line, " ");
            for (int i = 0; i < 3; i++) {
                unsigned vertexPointer = touint(strtok(NULL, "/")) - 1;
                VEC_NEXT(indices) = vertexPointer;
                unsigned texturePointer = touint(strtok(NULL, "/")) - 1;
                texture_uv_out[vertexPointer*2] = texture_uv.data[texturePointer*2];
                texture_uv_out[vertexPointer*2+1] = 1 - texture_uv.data[texturePointer*2+1];
                unsigned normPointer = touint(strtok(NULL, " ")) - 1;
                normals_out[vertexPointer*3]   = normals.data[normPointer*3];
                normals_out[vertexPointer*3+1] = normals.data[normPointer*3+1];
                normals_out[vertexPointer*3+2] = normals.data[normPointer*3+2];
            }
        }
    } while (my_getline(&line, &len, file) > 0);

    free(normals.data);    normals.data = NULL;
    free(texture_uv.data); texture_uv.data = NULL;

    out->vertices         = vertices.data;
    out->vertices_count   = vertices.count;
    out->texture_uv       = texture_uv_out;
    out->texture_uv_count = vertices.count;
    out->normals          = normals_out;
    out->normals_count    = vertices.count;
    out->indices          = indices.data;
    out->indices_count    = indices.count;
}

void free_obj_modeldata(struct ModelData *m)
{
    /* Safe to case away const since it was initialized as non-const */
    free((void *)m->vertices);
    free((void *)m->texture_uv);
    free((void *)m->normals);
    free((void *)m->indices);
}
