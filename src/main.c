#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <GL/glew.h>

#include "utils.h"
#include "glutils.h"

static const GLint WIDTH = 800, HEIGHT = 600;
static const GLchar *const VERTEXFILE = RESOURCE_DIR "vertex.glsl";
static const GLchar *const FRAGFILE   = RESOURCE_DIR "frag.glsl";

static SDL_Window *init_sdl(SDL_GLContext *context)
    ATTR((returns_nonnull, nonnull(1)));
static void cleanup_sdl(SDL_Window *window, SDL_GLContext context)
    ATTR((nonnull(1, 2)));

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;

    /* Init SDL and OpenGL */
    SDL_GLContext context;
    SDL_Window *window = init_sdl(&context);

    /* Set the OpenGL viewport to the entire window */
    GLCHECK(glViewport(0, 0, WIDTH, HEIGHT));

    /* Init shader program */
    GLuint program = load_program(VERTEXFILE, FRAGFILE);

    const GLfloat vertices[12] = {
        -0.5f, -0.5f, 0.0f, /* bottom left  */
         0.5f, -0.5f, 0.0f, /* bottom right */
        -0.5f,  0.5f, 0.0f, /* top left     */
         0.5f,  0.5f, 0.0f, /* top right    */
    };

    const GLfloat colors[12] = {
        1.0f, 0.0f, 0.0f, /* bottom left  */
        0.0f, 1.0f, 0.0f, /* bottom right */
        0.0f, 0.0f, 1.0f, /* top left     */
        0.2f, 0.2f, 0.2f  /* top right    */
    };

    const GLushort indices[6] = {
        0, 1, 2, /* First triangle  */
        2, 1, 3  /* Second triangle */
    };

    GLuint VAO, VBO, VBO2, EBO;

    /* Create Vertex Array */
    VAO = gen_array();

    /* Create Vertex Buffers */
    VBO = gen_buffer(GL_ARRAY_BUFFER, sizeof vertices, vertices);
    /* Set position to be variable in location 0 
     * 0 - generic vector index to modify
     * 3 - number of components per generic vector attribute
     * GL_FLOAT - type of each component
     * 6 * sizeof (GLfloat) - byte offset between consecutive attributes
     * 0 - offset from the first component
     */
    attrib_buffer(0, 3, GL_FLOAT, 3 * sizeof (GLfloat), 0);

    VBO2 = gen_buffer(GL_ARRAY_BUFFER, sizeof colors, colors);
    attrib_buffer(1, 3, GL_FLOAT, 3 * sizeof (GLfloat), 0);

    /* Create Element/Index Buffer */
    EBO = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices);

    /* Un-bind VAO */
    bind_array(0);

    int ret = SDL_GL_SetSwapInterval(1);
    ASSERT(ret == 0, SDL_GetError());

    /* Event Loop */
    SDL_Event e;
    while (1)
    {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT
                    || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_q)) {
                goto cleanup;
            }
        }

        GLCHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GLCHECK(glClear(GL_COLOR_BUFFER_BIT));

        /* Setup program & VAO */
        use_program(program);
        bind_array(VAO);

        /* Actually draw */
        GLCHECK(glDrawElements(GL_TRIANGLES, 
                    ARRAY_SIZE(indices), GL_UNSIGNED_SHORT, NULL));

        /* Remove program & VAO */
        use_program(0);
        bind_array(0);

        SDL_GL_SwapWindow(window);
    }

cleanup:
    // Shader_free(ourShader);
    del_program(program);
    del_array(VAO);
    del_buffer(VBO);
    del_buffer(VBO2);
    del_buffer(EBO);
    cleanup_sdl(window, context);

    return EXIT_SUCCESS;
}

static SDL_Window *init_sdl(SDL_GLContext *context)
{
    /* Initialize SDL2 */
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    ASSERT(ret == 0, SDL_GetError());
    /* Configure SDL2 to work with OpenGL 3.3 */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    /* Create an SDL window & OpenGL context */
    SDL_Window *window = SDL_CreateWindow("OpenGL Tutorial", 
            0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    ASSERT(window != NULL, SDL_GetError());
    *context = SDL_GL_CreateContext(window);
    ASSERT(*context != NULL, SDL_GetError());

    /* Initialize GLEW, which sets up OpenGL */
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    ASSERT(err == GLEW_OK, "Could not initialize GLEW");

    return window;
}

static void cleanup_sdl(SDL_Window *window, SDL_GLContext context)
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
