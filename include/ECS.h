#ifndef ECS_INCLUDED
#define ECS_INCLUDED

#include <GL/glew.h>
#include "cglm/cglm.h"
#include "components.h"


/* ---- Entity ---- */
/* represents a single item in the world */
typedef unsigned int Entity;
#define ENTITY_MAX_COUNT 100

/* ---- WorldState ---- */
/* the current state of the world not relating to entity components */
struct WorldState {
    struct {
        GLfloat pos[3];
        GLfloat color[3];
    } light;
    mat4 projection_matrix,
         view_matrix;
};

/* ---- World ---- */
/* a storage for all the entities and gamestate (should be singleton)
 * all systems map over this World */
typedef struct World {
    unsigned entity_count;

    Component mask[ENTITY_MAX_COUNT];
    Tag tags[ENTITY_MAX_COUNT];

    struct Renderable render[ENTITY_MAX_COUNT];
    struct OpenGL_GC opengl_gc[ENTITY_MAX_COUNT];
    struct Textured texture[ENTITY_MAX_COUNT];
    struct Colored color[ENTITY_MAX_COUNT];
    struct Displacement displacement[ENTITY_MAX_COUNT];
    struct Rotation rotation[ENTITY_MAX_COUNT];
    struct Velocity velocity[ENTITY_MAX_COUNT];

    struct WorldState state;
} World;
#define WORLD_INIT {.entity_count = 0, .mask = {0}, .tags = {0}}

/* Define getters for each member 
 * The are macros are used to preserve const-ness */
static inline Component get_mask(const World *w, Entity id) 
{ return w->mask[id]; }
static inline Tag get_tags(const World *w, Entity id)
{ return w->tags[id]; }
static inline unsigned entity_count(const World *w)
{ return w->entity_count; }
#define world_state(w)  (w->state)
#define get_render(w, id)       (&(w)->render[id])
#define get_gc(w, id)           (&(w)->opengl_gc[id])
#define get_texture(w, id)      (&(w)->texture[id])
#define get_color(w, id)        (&(w)->color[id])
#define get_displacement(w, id) (&(w)->displacement[id])
#define get_rotation(w, id)     (&(w)->rotation[id])
#define get_velocity(w, id)     (&(w)->velocity[id])

/* Systems operate over the world and return 0 on success */
typedef int (*System)(World *);
typedef int (*ConstSystem)(const World *);

/* Creating and destroying entities */
Entity create_entity(World *w);
void destroy_entity(World *w, Entity entity);

#endif
