#include <GL/glew.h>
#include "utils.h"
#include "glutils.h"
#include "render_system.h"
#include "components.h"
#include "ECS.h"
#include "kvec.h"
#include "objloader.h"

static const int RENDER_MASK = COMP_RENDERABLE | COMP_DISPLACEMENT | COMP_ROTATION;

static void render_textured_entity(const struct Renderable *rend, 
                                   const struct Rotation *rot, 
                                   const struct Displacement *disp, 
                                   const struct Textured *tex,
                                   const struct WorldState *state);
static void render_colored_entity(const struct Renderable *rend, 
                                  const struct Rotation *rot, 
                                  const struct Displacement *disp,
                                  const struct Colored *color,
                                  const struct WorldState *state);

/* render_system - System for rendering entities on screen
 * Components - Read-Only:
 *  - Renderable
 *  - Displacement
 *  - Rotation
 *  - Textured/Colored
 * Contracts:
 *  @world != NULL
 *  Not threadsafe - calls OpenGL functions
 */
int render_system(const World *world)
{
    size_t max_count = entity_count(world);
    const struct Displacement *disp = NULL;
    const struct Rotation *rot = NULL;
    const struct Renderable *render = NULL;
    const struct Textured *tex = NULL;
    const struct Colored *color = NULL;
    const struct WorldState *state = world_state(world);

    for (size_t i = 0, count = 0; i < ENTITY_MAX_COUNT && count < max_count; i++)
    {
        enum Component comp = get_mask(world, i);
        if (comp == COMP_NONE)
            continue;

        count++;
        if (!(comp & RENDER_MASK))
            continue;

        disp   = get_displacement(world, i);
        rot    = get_rotation(world, i);
        render = get_render(world, i);

        if (comp & COMP_TEXTURED) {
            tex = get_texture(world, i);
            render_textured_entity(render, rot, disp, tex, state);
        } else if (comp & COMP_COLORED) {
            color = get_color(world, i);
            render_colored_entity(render, rot, disp, color, state);
        } else {
            LOG("Entity %zu is Renderable, but has no color or texture\n", i);
            return 1;
        }
    }
    return 0;
}

static void create_model_matrix(mat4 out, 
        const struct Displacement *disp,
        const struct Rotation *rot)
{
    glm_mat4_identity(out);

    glm_translate_x(out, disp->x);
    glm_translate_y(out, disp->y);
    glm_translate_z(out, disp->z);

    glm_rotate_x(out, rot->x, out);
    glm_rotate_y(out, rot->y, out);
    glm_rotate_z(out, rot->z, out);

    glm_scale_uni(out, disp->scale);
}

static void render_textured_entity(const struct Renderable *rend, 
                                   const struct Rotation *rot, 
                                   const struct Displacement *disp, 
                                   const struct Textured *tex,
                                   const struct WorldState *state)
{
    /* Set up using the rendering program and VAO */
    use_program(rend->program);
    bind_array(rend->VAO);

    /* Set up the texture */
    GLCHECK(glActiveTexture(GL_TEXTURE0));
    bind_texture(tex->texture);

    /* Get locations for given uniforms, or -1 if that uniform doesn't exist */
    GLuint model_location = uniform_loc(rend->uniforms, "model");
    GLuint view_location  = uniform_loc(rend->uniforms, "view");
    GLuint proj_location  = uniform_loc(rend->uniforms, "projection");

    /* Set the required uniforms */
    if (model_location != -1) {
        mat4 model_matrix;
        create_model_matrix(model_matrix, disp, rot);
        GLCHECK(glUniformMatrix4fv(model_location, 1, GL_FALSE, model_matrix[0]));
    }
    if (view_location != -1) {
        GLCHECK(glUniformMatrix4fv(view_location, 1, GL_FALSE, state->view_matrix[0]));
    }
    if (proj_location != -1) {
        GLCHECK(glUniformMatrix4fv(proj_location, 1, GL_FALSE, state->projection_matrix[0]));
    }

/* TODO: implement lighting
 * GLCHECK(glUniform3fv(m->light_pos_uniform, 1, light_pos));
 * GLCHECK(glUniform3fv(m->light_color_uniform, 1, light_color));
 */

    GLCHECK(glDrawElements(GL_TRIANGLES, rend->num_indices, GL_UNSIGNED_INT, NULL));

    bind_texture(0);
    use_program(0);
    bind_array(0);
}

static void render_colored_entity(const struct Renderable *rend, 
                                  const struct Rotation *rot, 
                                  const struct Displacement *disp,
                                  const struct Colored *color,
                                  const struct WorldState *state)
{
    FATAL("TODO: implement rendering colored entities");
    (void)rend, (void)rot, (void)disp, (void)color, (void)state;
}

static const GLuint VERT_POS = 0,
                    TEX_POS  = 1,
                    NORM_POS = 2;
static const float FOV        = 70;
static const float NEAR_PLANE = 0.1f;
static const float FAR_PLANE  = 1000.0f;
static const float ASPECT     = 800.0f / 600.0f;

int make_renderable(World *world, Entity e, const char *objfile,
        const char *vertexfile, const char *fragfile, const char *texturefile)
{
    int mask = get_mask(world, e);
    if (!(mask & (COMP_DISPLACEMENT | COMP_ROTATION))) {
        LOG("Trying to create renderable without dependent fields");
        return 1;
    }

    struct Renderable *rend = get_render(world, e);
    rend->uniforms = kh_init(uniforms);
    struct OpenGL_GC *gc = get_gc(world, e);
    kv_init(gc->buffers);

    struct ModelData data;
    load_obj(objfile, &data);

    /* Create the program */
    rend->program = load_program(vertexfile, fragfile);
    /* Create VAO */
    rend->VAO = gen_array();

    /* Create vertex buffers for vertices, texture coords, normals, indices */
    /* Vertices */
    *kv_pushp(GLuint, gc->buffers) = gen_buffer(GL_ARRAY_BUFFER, 
            data.vertices_count * sizeof (GLfloat), data.vertices);
    attrib_buffer(VERT_POS, 3, GL_FLOAT, sizeof (GLfloat) * 3, 0);
    /* Texture coords */
    *kv_pushp(GLuint, gc->buffers) = gen_buffer(GL_ARRAY_BUFFER, 
            data.texture_uv_count * sizeof (GLfloat), data.texture_uv);
    attrib_buffer(TEX_POS, 2, GL_FLOAT, sizeof (GLfloat) * 2, 0);
    /* Normals */
    *kv_pushp(GLuint, gc->buffers) = gen_buffer(GL_ARRAY_BUFFER, 
            data.normals_count * sizeof (GLfloat), data.normals);
    attrib_buffer(NORM_POS, 3, GL_FLOAT, sizeof (GLfloat) * 3, 0);
    /* Indices */
    *kv_pushp(GLuint, gc->buffers) = gen_buffer(GL_ELEMENT_ARRAY_BUFFER,
            data.indices_count * sizeof (GLuint), data.indices);

    free_obj_modeldata(&data);

    /* Load up the uniforms hashtable */
    int ret;
    for (size_t i = 0; i < NUM_UNIFORMS; i++) {
        khiter_t iter = kh_put(uniforms, rend->uniforms, UNIFORMS[i], &ret);
        ASSERT(ret != -1, "Error inserting uniform into hashtable");
        kh_val(rend->uniforms, iter) = glGetUniformLocation(rend->program, UNIFORMS[i]);
    }
}

int collect_garbage(World *world, Entity e)
{
    struct Renderable *rend = get_render(world, e);
    struct OpenGL_GC *gc = get_gc(world, e);

    del_program(rend->program);
    del_array(rend->VAO);

    GLuint VBO;
    while (kv_size(gc->buffers) != 0) {
        VBO = kv_pop(gc->buffers);
        del_buffer(VBO);
    }
    kv_destroy(gc->buffers);
}
