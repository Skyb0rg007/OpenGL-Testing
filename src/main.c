#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>

#include "utils.h"
#include "glutils.h"
#include "cube.h"
#include "entity.h"
#include "objloader.h"

static const GLint WIDTH = 800, HEIGHT = 600;
static const Uint32 SDL_FLAGS = SDL_INIT_VIDEO;
static const int    IMG_FLAGS = IMG_INIT_PNG;

static void init_sdl(SDL_Window **w, SDL_GLContext *ctx)           ATTR((nonnull(1,2)));
static void cleanup_sdl(SDL_Window *window, SDL_GLContext context) ATTR((nonnull(1, 2)));
static void handle_inputs(struct Entity *cube)                     ATTR((nonnull(1)));
static int log_cube(void *arg);

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;

    /* Init SDL and OpenGL */
    SDL_GLContext context;
    SDL_Window *window;
    init_sdl(&window, &context);

    /* Set the OpenGL viewport to the entire window */
    GLCHECK(glViewport(0, 0, WIDTH, HEIGHT));

    /* Enable depth testing */
    GLCHECK(glEnable(GL_DEPTH_TEST));

    /* Limit swap interval */
    int ret = SDL_GL_SetSwapInterval(1);
    ASSERT(ret == 0, SDL_GetError());

    /* Create object models */
    struct Model dragonmodel;
    load_obj_model(RESOURCE_DIR "dragon.obj",
                   NULL,
                   RESOURCE_DIR "entity.vertex.glsl",
                   RESOURCE_DIR "entity.fragment.glsl",
                   &dragonmodel);

    struct Entity dragons[10];

restart:
    srand(time(NULL));
    for (size_t i = 0; i < ARRAY_SIZE(dragons); i++) {
        dragons[i].x     =  (rand() % 18) - 9;
        dragons[i].y     =  (rand() % 18) - 9;
        dragons[i].z     = -(rand() % 20) - 5;
        dragons[i].rot_x = (rand() % 3) - 1;
        dragons[i].rot_y = (rand() % 3) - 1;
        dragons[i].rot_z = (rand() % 3) - 1;
        dragons[i].scale =  0.3f;
    }

    /* Event Loop */
    SDL_Event e;
    while (1)
    {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT
                    || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_q)) {
                goto cleanup;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                goto restart;
            }
        }

        SDL_PumpEvents();

        GLCHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GLCHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (size_t i = 0; i < ARRAY_SIZE(dragons); i++)
            render_entity(&dragonmodel, &dragons[i]);

        SDL_GL_SwapWindow(window);
    }

cleanup:
    destroy_model(&dragonmodel);
    cleanup_sdl(window, context);

    return EXIT_SUCCESS;
}

static void init_sdl(SDL_Window **window, SDL_GLContext *context)
{
    /* Initialize SDL2 for handling video */
    int ret = SDL_Init(SDL_FLAGS);
    ASSERT(ret == 0, SDL_GetError());

    /* Configure SDL2 to work with OpenGL 3.3 */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG|SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    /* Create an SDL window & OpenGL context */
    *window = SDL_CreateWindow("OpenGL Tutorial", 
            0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    ASSERT(*window != NULL, SDL_GetError());
    *context = SDL_GL_CreateContext(*window);
    ASSERT(*context != NULL, SDL_GetError());

    /* Initialize GLEW, which sets up OpenGL */
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    ASSERT(err == GLEW_OK, glewGetErrorString(err));

    /* Initialize SDL_image for loading PNGs */
    ret = IMG_Init(IMG_FLAGS);
    ASSERT((ret & IMG_FLAGS) == IMG_FLAGS, IMG_GetError());
}

static void cleanup_sdl(SDL_Window *window, SDL_GLContext context)
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

ATTR((unused))
static void handle_inputs(struct Entity *cube)
{
    static const Uint8 *keyboardStates = NULL;

    if (keyboardStates == NULL)
        keyboardStates = SDL_GetKeyboardState(NULL);

    if (keyboardStates[SDL_SCANCODE_UP])
        cube->rot_x += -0.04f;
    if (keyboardStates[SDL_SCANCODE_DOWN])
        cube->rot_x += 0.04f;
    if (keyboardStates[SDL_SCANCODE_LEFT])
        cube->rot_y += -0.04f;
    if (keyboardStates[SDL_SCANCODE_RIGHT])
        cube->rot_y += 0.04f;
    if (keyboardStates[SDL_SCANCODE_COMMA])
        cube->rot_z += 0.04f;
    if (keyboardStates[SDL_SCANCODE_PERIOD])
        cube->rot_z += -0.04f;
    if (keyboardStates[SDL_SCANCODE_EQUALS])
        cube->scale += 0.04f;
    if (keyboardStates[SDL_SCANCODE_MINUS])
        cube->scale += -0.04f;
    if (keyboardStates[SDL_SCANCODE_W])
        cube->y += 0.04f;
    if (keyboardStates[SDL_SCANCODE_S])
        cube->y += -0.04f;
    if (keyboardStates[SDL_SCANCODE_A])
        cube->x += -0.04f;
    if (keyboardStates[SDL_SCANCODE_D])
        cube->x += 0.04f;
    if (keyboardStates[SDL_SCANCODE_F])
        cube->z += 0.04f;
    if (keyboardStates[SDL_SCANCODE_G])
        cube->z += -0.04f;
}

ATTR((unused))
static int log_cube(void *arg)
{
    const struct Entity *const c = arg;
    while (1) {
        printf("{\n\t.x = %f, .y = %f, .z = %f,\n"
                "\t.rotx = %f, .roty = %f, .rotz = %f\n"
                "\t.scale = %f\n"
                "}\n", 
                c->x, c->y, c->z, c->rot_x, c->rot_y, c->rot_z, c->scale);
        SDL_Delay(3 * 1000);
    }

    return 0;
}
