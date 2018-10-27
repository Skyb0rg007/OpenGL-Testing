#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include <GL/glew.h>
#include "kvec.h"
#include "utils.h"

/* Bitmask for components */
typedef enum Component {
    COMP_NONE         = 0,
    COMP_RENDERABLE   = 1u << 0,
    COMP_OPENGL_GC    = 1u << 1,
    COMP_TEXTURED     = 1u << 2,
    COMP_COLORED      = 1u << 3,
    COMP_DISPLACEMENT = 1u << 4,
    COMP_ROTATION     = 1u << 5,
    COMP_VELOCITY     = 1u << 6,
    COMP_ILLUMINATED  = 1u << 7,
} Component;

/* Bitmask for tags */
typedef enum Tag {
    TAG_NONE = 0,
} Tag;

/* ---- Components ---- */

/* Renderable - component to enable rendering of an entity
 * @program: the id of the entity's shader program
 * @VAO: the id of the entity's VAO
 * Deps: Displacement, Rotation, Textured | Colored
 */
struct Renderable {
    GLuint program;
    GLuint VAO;
    GLsizei num_indices;
    struct UniformsCommon {
        GLint model_matrix,
              view_matrix,
              projection_matrix;
    } uniforms;
};

/* OpenGL_GC - what gets freed at the end of an entity's lifetime
 * @buffers: vector containing ids of VBOs and EBOs to free
 *  ex. indices, vertices, texture_u, normals
 * Added when an Entity is made renderable
 */
struct OpenGL_GC {
    kvec_t(GLuint) buffers;
};

/* Textured - component to enable an entity to have a texture
 * @texture: id of the entity's texture
 */
struct Textured {
    GLuint texture;
};

/* Colored - component to enable coloring of an entity
 * @r, g, b: red blue and green values
 */
struct Colored {
    GLfloat r, g, b;
};

/* Displacement - component to enable an entity to have a position
 * @x, y, z: location of the entity
 * @scale: the size of the entity
 */
struct Displacement {
    GLfloat x, y, z, scale;
};

/* Rotation - component to enable rotating an entity
 * @x, y, z: rotation of the entity in a given direction
 */
struct Rotation {
    GLfloat x, y, z;
};

/* Velocity - component to enable an entity to have a velocity
 * @x, y, z: velocity in the given direction
 */
struct Velocity {
    GLfloat x, y, z;
};

/* Illuminated - component to enable an entity to give off light
 * @intensity: the intensity of the light
 */
struct Illuminated {
    GLfloat intensity;
};

#endif
