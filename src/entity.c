#include <GL/glew.h>
#include <cglm/cglm.h>
#include "entity.h"
#include "glutils.h"

static const GLuint VERT_POS = 0,
                    TEX_POS  = 1,
                    NORM_POS = 2;
static const float FOV        = 70;
static const float NEAR_PLANE = 0.1f;
static const float FAR_PLANE  = 1000.0f;
static const float ASPECT     = 800.0f / 600.0f;

/* create_model - generate a Model from ModelData
 * @data: structure containing information about creating Models
 * @out: the Model to initialize
 *
 * Contracts:
 *  - All parameters are non-null and previously allocated
 *  - All @data fields are initialized
 *  - Not threadsafe - calls OpenGL functions
 * Responsibilities:
 *  - Call delete_model() on the Model after use
 */
void create_model(const struct ModelData *data, struct Model *out)
{
    out->program = load_program(data->vert_filepath, data->frag_filepath);

    out->vao = gen_array();

    out->vbo_vertices = gen_buffer(GL_ARRAY_BUFFER, 
            data->vertices_count * sizeof (GLfloat), data->vertices);
    attrib_buffer(VERT_POS, 3, GL_FLOAT, sizeof (GLfloat) * 3, 0);

    if (data->tex_filepath) {
        out->vbo_texture_uv = gen_buffer(GL_ARRAY_BUFFER, 
                data->texture_uv_count * sizeof (GLfloat), data->texture_uv);
        attrib_buffer(TEX_POS, 2, GL_FLOAT, sizeof (GLfloat) * 2, 0);

        out->texture = load_texture(data->tex_filepath);
    } else {
        out->vbo_texture_uv = 0;
        out->texture = 0;
    }

    out->vbo_normals = gen_buffer(GL_ARRAY_BUFFER, 
            data->normals_count * sizeof (GLfloat), data->normals);
    attrib_buffer(NORM_POS, 3, GL_FLOAT, sizeof (GLfloat) * 3, 0);

    out->ebo_indices = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, 
            data->indices_count * sizeof (GLuint), data->indices);

    out->model_uniform       = glGetUniformLocation(out->program, "model");
    out->view_uniform        = glGetUniformLocation(out->program, "view");
    out->projection_uniform  = glGetUniformLocation(out->program, "projection");
    out->light_pos_uniform   = glGetUniformLocation(out->program, "light_pos");
    out->light_color_uniform = glGetUniformLocation(out->program, "light_color");

    out->num_indices = data->indices_count;

    bind_array(0);
    bind_texture(0);
}

/* destroy_model - destroy Model previously allocated with create_model()
 * @model: the Model
 *
 * Contracts:
 *  - @model was previously allocated with create_model()
 *  - Not threadsafe - calls OpenGL functions
 */
void destroy_model(const struct Model *model)
{
    del_program(model->program);
    del_array(model->vao);
    del_buffer(model->vbo_vertices);
    del_buffer(model->vbo_normals);
    del_buffer(model->ebo_indices);
    if (model->vbo_texture_uv != 0){
        del_buffer(model->vbo_texture_uv);
        del_texture(model->texture);
    }
    /* Don't need to do anything to uniforms */
}

/* render_entity - render an entity given a model
 *  - see render_entities
 */
void render_entity(const struct Model *model, const struct Entity *entity)
{
    render_entities(model, entity, 1);
}

/* render_entities - render an array of Entities given a model
 * @m: the Model to use
 * @entity: pointer to the first Entity
 * @n: number of Entities in the array
 *
 * Contracts:
 *  - @m and @entity are non-null and previously allocated + set-up
 *  - @entity has @n elements
 *  - Not threadsafe - calls OpenGL functions
 */
void render_entities(const struct Model *m, const struct Entity *entity, size_t n)
{
    use_program(m->program);
    bind_array(m->vao);
    if (m->texture != 0) {
        /* Set up texture uniform */
        GLCHECK(glActiveTexture(GL_TEXTURE0));
        bind_texture(m->texture);
    }

    /* Set up view matrix */
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

    /* Set up projection matrix */
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(FOV, ASPECT, NEAR_PLANE, FAR_PLANE, projection);

    static const vec3 light_pos   = {0.0f, 0.0f, 0.0f};
    static const vec3 light_color = {1.0f, 1.0f, 1.0f};

    for (size_t i = 0; i < n; i++) {
        /* Set up model matrix */
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        glm_translate_x(model, entity->x);
        glm_translate_y(model, entity->y);
        glm_translate_z(model, entity->z);
        glm_rotate_x(model, entity->rot_x, model);
        glm_rotate_y(model, entity->rot_y, model);
        glm_rotate_z(model, entity->rot_z, model);
        glm_scale_uni(model, entity->scale);

        GLCHECK(glUniformMatrix4fv(m->model_uniform, 1, GL_FALSE, model[0]));
        GLCHECK(glUniformMatrix4fv(m->view_uniform, 1, GL_FALSE, view[0]));
        GLCHECK(glUniformMatrix4fv(m->projection_uniform, 1, GL_FALSE, projection[0]));
        GLCHECK(glUniform3fv(m->light_pos_uniform, 1, light_pos));
        GLCHECK(glUniform3fv(m->light_color_uniform, 1, light_color));
        GLCHECK(glDrawElements(GL_TRIANGLES, m->num_indices, GL_UNSIGNED_INT, NULL));
    }

    use_program(0);
    bind_array(0);
    if (m->texture != 0)
        bind_texture(0);
}
