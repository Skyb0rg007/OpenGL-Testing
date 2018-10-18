#ifndef TERRAINS_H_INCLUDED
#define TERRAINS_H_INCLUDED

#include "glutils.h"
#include "entity.h"

struct Terrain {
    struct TerrainPos *pos;
    struct Model *model;
};

struct TerrainPos {
    float x, z;
};

void load_terrain_model(const char *texturefile, const char *vertexfile,
        const char *fragmentfile, struct Model *m);
void free_terrain_modeldata(struct ModelData *m);
void generate_terrain(struct ModelData *out);

#endif
