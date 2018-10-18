#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED

#include "entity.h"

/* fills out:
 *  m->vertices
 *  m->vertices_count
 *  m->texture_uv
 *  m->texture_uv_count
 *  m->normals
 *  m->normals_count
 *  m->indices
 *  m->indices_count
 *********
 * does not touch:
 *  m->vert_filepath
 *  m->frag_filepath
 *  m->tex_filepath
 */
void load_obj(const char *filepath, struct ModelData *out) ATTR((nonnull(1,2)));
/* frees the members that were initialized with load_obj */
void free_obj_modeldata(struct ModelData *m);

void load_obj_model(const char *objfile, const char *texturefile, 
        const char *vertexfile, const char *fragmentfile, struct Model *m)
    ATTR((nonnull(1,3,4,5)));

#endif
