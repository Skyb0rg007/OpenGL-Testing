#ifndef RENDER_SYSTEM_H_INCLUDED
#define RENDER_SYSTEM_H_INCLUDED

#include <GL/glew.h>
#include "ECS.h"
#include "utils.h"

int render_system(const World *);

/* If texturefile is NULL, then the entity is colored */
int make_renderable(World *world, Entity e, const char *objfile,
        const char *vertexfile, const char *fragfile, const char *texturefile)
    ATTR((nonnull(1, 3, 4, 5)));

int collect_garbage(World *world, Entity e);

#endif
