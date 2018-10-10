#include <GL/glew.h>
#include <cglm/cglm.h>
#include "entity.h"
#include "glutils.h"

static const GLuint VERT_POS = 0,
                    TEX_POS  = 1,
                    NORM_POS = 2;

static void gen_matrix(const struct Entity *entity, mat4 matrix) 
    ATTR((nonnull(1, 2)));

void create_model(const struct ModelData *data, struct Model *out)
{
    out->program = load_program(data->vert_filepath, data->frag_filepath);

    out->vao = gen_array();

    out->vbo_vertices = gen_buffer(GL_ARRAY_BUFFER, 
            data->vertices_count * sizeof (GLfloat), data->vertices);
    attrib_buffer(VERT_POS, 3, GL_FLOAT, sizeof (GLfloat) * 3, 0);

    out->vbo_texture_uv = gen_buffer(GL_ARRAY_BUFFER, 
            data->texture_uv_count * sizeof (GLfloat), data->texture_uv);
    attrib_buffer(TEX_POS, 2, GL_FLOAT, sizeof (GLfloat) * 2, 0);

    out->vbo_normals = gen_buffer(GL_ARRAY_BUFFER, 
            data->normals_count * sizeof (GLfloat), data->normals);
    attrib_buffer(NORM_POS, 3, GL_FLOAT, sizeof (GLfloat) * 3, 0);

    out->ebo_indices = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, 
            data->indices_count * sizeof (GLuint), data->indices);

    out->texture = load_texture(data->tex_filepath);

    out->matrix_uniform = glGetUniformLocation(out->program, "modificationMatrix");

    out->num_indices = data->indices_count;

    bind_array(0);
    bind_texture(0);
}

void destroy_model(const struct Model *model)
{
    del_program(model->program);
    del_array(model->vao);
    del_buffer(model->vbo_vertices);
    del_buffer(model->vbo_texture_uv);
    del_buffer(model->vbo_normals);
    del_buffer(model->ebo_indices);
    del_texture(model->texture);
    /* Don't need to do anything to matrix_uniform */
}

ATTR((nonnull(1, 2)))
static void gen_matrix(const struct Entity *entity, mat4 matrix)
{
    static const float FOV = 70;
    static const float NEAR_PLANE = 0.1f;
    static const float FAR_PLANE = 1000.0f;
    static const float ASPECT = 800.0f / 600.0f;

    /* Set up model matrix */
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate_x(model, entity->x);
    glm_translate_y(model, entity->y);
    glm_translate_z(model, entity->z);
    glm_rotate_x(model, entity->rot_x, model);
    glm_rotate_y(model, entity->rot_y, model);
    glm_rotate_z(model, entity->rot_z, model);
    glm_scale_uni(model, entity->scale);

    /* Set up view matrix */
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

    /* Set up projection matrix */
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(FOV, ASPECT, NEAR_PLANE, FAR_PLANE, projection);

    /* Create the final matrix to send to the shader by multiplying
       the model, view, and projection matrices */
    glm_mat4_mulN((mat4 *[]){&projection, &view, &model}, 3, matrix);
}

void render_entity(const struct Model *model, const struct Entity *entity)
{
    render_entities(model, entity, 1);
}

void render_entities(const struct Model *m, const struct Entity *entity, size_t n)
{
    mat4 matrix;

    use_program(m->program);
    bind_array(m->vao);
    GLCHECK(glActiveTexture(GL_TEXTURE0));
    bind_texture(m->texture);

    for (size_t i = 0; i < n; i++) {
        gen_matrix(entity + i, matrix);
        GLCHECK(glUniformMatrix4fv(m->matrix_uniform, 1, GL_FALSE, matrix[0]));
        GLCHECK(glDrawElements(GL_TRIANGLES, m->num_indices, GL_UNSIGNED_INT, NULL));
    }

    use_program(0);
    bind_array(0);
    bind_texture(0);
}
