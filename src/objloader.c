#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <GL/glew.h>
#include <kvec.h>

#include "glutils.h"
#include "objloader.h"
#include "utils.h"

#define MAX_LINE_LEN 256

/* load_obj - create ModelData from an .obj file
 * @filepath: path to the .obj file
 * @out: allocated ModelData to fill
 *
 * Contracts: 
 *  - @filepath is a valid .obj file
 *  - @out is an allocated ModelData structure
 *  - Not threadsafe - it uses strtok() and strerror()
 * Responsibilities:
 *  - Call free_obj_modeldata() on @out after use
 *
 *  TODO: make threadsafe - replace strtok() and strerror()
 */
void load_obj(const char *filepath, struct ModelData *out)
{
    kvec_t(GLfloat) vertices   = KVEC_INIT;
    kvec_t(GLfloat) normals    = KVEC_INIT;
    kvec_t(GLfloat) texture_uv = KVEC_INIT;

    FILE *file = fopen(filepath, "rt");
    ASSERT(file != NULL, "fopen: %s", strerror(errno));

    char line[MAX_LINE_LEN];

    /* Load vertices, texture-coords, and normals */
    while (fgets(line, sizeof line, file) != NULL) 
    {
        if (strncmp(line, "v ", 2) == 0) {
            *kv_pushp(GLfloat, vertices) = tofloat(strtok(line+2, " "));
            *kv_pushp(GLfloat, vertices) = tofloat(strtok(NULL, " "));
            *kv_pushp(GLfloat, vertices) = tofloat(strtok(NULL, " "));
        } else if (strncmp(line, "vt ", 3) == 0) {
            *kv_pushp(GLfloat, texture_uv) = tofloat(strtok(line+3, " "));
            *kv_pushp(GLfloat, texture_uv) = tofloat(strtok(NULL, " "));
        } else if (strncmp(line, "vn ", 3) == 0) {
            *kv_pushp(GLfloat, normals) = tofloat(strtok(line+3, " "));
            *kv_pushp(GLfloat, normals) = tofloat(strtok(NULL, " "));
            *kv_pushp(GLfloat, normals) = tofloat(strtok(NULL, " "));
        } else if (strncmp(line, "f ", 2) == 0) {
            break;
        } else if (strncmp(line, "# ", 2) != 0) {
            fprintf(stderr, "Could not parse line \"%s\": ignoring\n", line);
        }
    }

    /* Get ready to read the object's indices */
    kvec_t(unsigned) indices = KVEC_INIT;

    /* Allocate output array for normals and texture_uv */
    float *normals_out    = calloc(kv_size(vertices), sizeof (GLfloat));
    float *texture_uv_out = calloc(kv_size(vertices), sizeof (GLfloat));

    do {
        if (strncmp(line, "f ", 2) == 0) {
            strtok(line, " ");
            for (int i = 0; i < 3; i++) {
                unsigned vertexPointer = touint(strtok(NULL, "/")) - 1;
                *kv_pushp(unsigned, indices) = vertexPointer;
                unsigned texturePointer = touint(strtok(NULL, "/")) - 1;
                texture_uv_out[vertexPointer*2] = kv_A(texture_uv, texturePointer*2);
                texture_uv_out[vertexPointer*2+1] = 1 - kv_A(texture_uv, texturePointer*2+1);
                unsigned normPointer = touint(strtok(NULL, " ")) - 1;
                normals_out[vertexPointer*3]   = kv_A(normals, normPointer*3);
                normals_out[vertexPointer*3+1] = kv_A(normals, normPointer*3+1);
                normals_out[vertexPointer*3+2] = kv_A(normals, normPointer*3+2);
            }
        }
    } while (fgets(line, sizeof (line), file) != NULL);

    kv_destroy(normals);
    kv_destroy(texture_uv);

    out->vertices_count   = kv_size(vertices);
    out->texture_uv_count = kv_size(vertices);
    out->normals_count    = kv_size(vertices);
    out->indices_count    = kv_size(indices);

    out->vertices         = vertices.a;
    out->texture_uv       = texture_uv_out;
    out->normals          = normals_out;
    out->indices          = indices.a;
}

/* free_obj_modeldata - free data allocated with load_obj()
 * @m: ModelData previously allocated with load_obj()
 * Contracts:
 *  - m was previously allocated with load_obj() or is NULL
 */
void free_obj_modeldata(struct ModelData *m)
{
    if (m != NULL) {
        /* Safe to case away const since it was initialized as non-const */
        free((void *)m->vertices);
        free((void *)m->texture_uv);
        free((void *)m->normals);
        free((void *)m->indices);
    }
}

/* load_obj_model - create a Model from a set of resource files
 * @objfile: the .obj file to load vertices, normals, etc
 * @texturefile: the .png file with the model's textures
 * @vertexfile: the model's vertex shader
 * @fragmentfile: the model's fragment shader
 * @m: the Model to load the data into
 *
 * Contracts:
 *  - All parameters are valid, allocated memory
 *    - Except texturefile, which can be NULL
 *  - The file parameters point to valid files
 *  - Not threadsafe - calls load_obj() 
 * Responsibilities:
 *  - Call destroy_model() on the Model structure after use
 */
/* void load_obj_model(const char *objfile, const char *texturefile, 
        const char *vertexfile, const char *fragmentfile, struct Model *m)
{
    struct ModelData data = {
        .vert_filepath = vertexfile,
        .frag_filepath = fragmentfile,
        .tex_filepath  = texturefile
    };
    load_obj(objfile, &data);
    create_model(&data, m);
    free_obj_modeldata(&data);
} */
