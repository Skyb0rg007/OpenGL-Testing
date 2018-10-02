#include <GL/glew.h>
#include <SDL2/SDL.h>

/* Don't call SDL_Quit() when debugging */

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;

    SDL_Init(SDL_INIT_VIDEO);
    /* SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); */
    /* SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); */
    /* SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); */
    /* SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); */
    /* SDL_Window *w = SDL_CreateWindow("Test", 0, 0, 800, 600, SDL_WINDOW_OPENGL); */
    /* SDL_GLContext ctx = SDL_GL_CreateContext(w); */

    /* glewInit(); */

    /* SDL_GL_DeleteContext(ctx); */
    /* SDL_DestroyWindow(w); */
    /* SDL_Quit(); */
    return 0;
}
