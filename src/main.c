#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "shader.h"
#include "utils.h"

static const GLint WIDTH = 800, HEIGHT = 600;
static const GLchar *const VERTEXFILE = RESOURCE_DIR "vertex.glsl";
static const GLchar *const FRAGFILE   = RESOURCE_DIR "frag.glsl";

static SDL_Window *init_sdl(SDL_GLContext *context);
static void cleanup_sdl(SDL_Window *window, SDL_GLContext context);

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;

    /* Init SDL and OpenGL */
    SDL_GLContext context;
    SDL_Window *window = init_sdl(&context);

    /* Set the OpenGL viewport to the entire window */
    $glcheck(glViewport(0, 0, WIDTH, HEIGHT));

    /* Init shaders */
    Shader *ourShader = Shader_new(VERTEXFILE, FRAGFILE);

    const GLfloat vertices[24] = {
        /*    position             color     */
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, /* bottom left  */
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, /* bottom right */
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, /* top left     */
         0.5f,  0.5f, 0.0f,   0.2f, 0.2f, 0.2f  /* top right    */
    };

    const GLushort indicies[6] = {
        0, 1, 2, /* First triangle  */
        2, 1, 3  /* Second triangle */
    };

    GLuint VAO, VBO, EBO;

    /* Create Vertex Array */
    $glcheck(glGenVertexArrays(1, &VAO));
    $glcheck(glBindVertexArray(VAO));

    /* Create Vertex Buffer */
    $glcheck(glGenBuffers(1, &VBO));
    $glcheck(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    $glcheck(glBufferData(GL_ARRAY_BUFFER, 
                sizeof vertices, vertices, GL_STATIC_DRAW));

    /* Create Element/Index Buffer */
    $glcheck(glGenBuffers(1, &EBO));
    $glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    $glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                sizeof indicies, indicies, GL_STATIC_DRAW));

    /* Set position to be variable in location 0 
     * 0 - generic vector index to modify
     * 3 - number of components per generic vector attribute
     * GL_FLOAT - type of each component
     * GL_FALSE - the data values are not normalized
     * 6*sizeof GL_FLOAT - byte offset between consecutive attributes
     * (const GLvoid *)0 - offset from the first component
     */
    $glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                6 * sizeof GL_FLOAT, (const GLvoid *)0));
    $glcheck(glEnableVertexAttribArray(0));

    /* Set color to be variable in location 1 */
    $glcheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                6 * sizeof GL_FLOAT, (const GLvoid *)(3 * sizeof (GLfloat))));
    $glcheck(glEnableVertexAttribArray(1));

    /* Un-bind VAO, VBO, and EBO */
    $glcheck(glBindVertexArray(0));
    $glcheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    $glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    int ret = SDL_GL_SetSwapInterval(1);
    $assert(ret == 0, SDL_GetError());

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

        $glcheck(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        $glcheck(glClear(GL_COLOR_BUFFER_BIT));

        /* draw OpenGL */
        /* Setup shader, VAO, EBO */
        Shader_use(ourShader);
        $glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
        $glcheck(glBindVertexArray(VAO));

        /* Actually draw */
        $glcheck(glDrawElements(GL_TRIANGLES, 
                    ARRAY_SIZE(indicies), GL_UNSIGNED_SHORT, NULL));

        /* Remove shader, VAO, EBO */
        $glcheck(glUseProgram(0));
        $glcheck(glBindVertexArray(0));
        $glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        SDL_GL_SwapWindow(window);
    }

cleanup:
    Shader_free(ourShader);
    $glcheck(glDeleteVertexArrays(1, &VAO));
    $glcheck(glDeleteBuffers(1, &VBO));
    $glcheck(glDeleteBuffers(1, &EBO));
    cleanup_sdl(window, context);

    return EXIT_SUCCESS;
}

static SDL_Window *init_sdl(SDL_GLContext *context)
{
    /* Initialize SDL2 */
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    $assert(ret == 0, SDL_GetError());
    /* Configure SDL2 to work with OpenGL 3.3 */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    /* Create an SDL window & OpenGL context */
    SDL_Window *window = SDL_CreateWindow("OpenGL Tutorial", 
            0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    $assert(window != NULL, SDL_GetError());
    *context = SDL_GL_CreateContext(window);
    $assert(*context != NULL, SDL_GetError());

    /* Initialize GLEW, which sets up OpenGL */
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    $assert(err == GLEW_OK, "Could not initialize GLEW");

    return window;
}

static void cleanup_sdl(SDL_Window *window, SDL_GLContext context)
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
