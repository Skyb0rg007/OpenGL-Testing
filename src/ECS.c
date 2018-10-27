#include <stdio.h>
#include <limits.h>
#include "utils.h"
#include "ECS.h"

Entity create_entity(World *world)
{
    for (Entity entity = 0; entity < ENTITY_MAX_COUNT; entity++) {
        if (world->mask[entity] == COMP_NONE) {
            return entity;
        }
    }
    FATAL("No entities left!\n");
    return UINT_MAX;
}

void destroy_entity(World *world, Entity entity)
{
    world->mask[entity] = COMP_NONE;
}
