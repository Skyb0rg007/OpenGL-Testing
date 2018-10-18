#include <SDL.h>
#include "camera.h"

void move_camera(struct Camera *cam)
{
    static const Uint8 *keyboardStates = NULL;

    if (keyboardStates == NULL)
        keyboardStates = SDL_GetKeyboardState(NULL);

    if (keyboardStates[SDL_SCANCODE_W])
        cam->y += 0.04f;
    if (keyboardStates[SDL_SCANCODE_S])
        cam->y -= 0.04f;
    if (keyboardStates[SDL_SCANCODE_D])
        cam->x += 0.04f;
    if (keyboardStates[SDL_SCANCODE_A])
        cam->x -= 0.04f;
    if (keyboardStates[SDL_SCANCODE_F])
        cam->z += 0.04f;
    if (keyboardStates[SDL_SCANCODE_G])
        cam->z -= 0.04f;

    if (keyboardStates[SDL_SCANCODE_E])
        cam->roll += 0.04f;
    if (keyboardStates[SDL_SCANCODE_Q])
        cam->roll += -0.04f;

    if (keyboardStates[SDL_SCANCODE_UP])
        cam->pitch -= 0.04f;
    if (keyboardStates[SDL_SCANCODE_DOWN])
        cam->pitch += 0.04f;
    if (keyboardStates[SDL_SCANCODE_LEFT])
        cam->rot_y += -0.04f;
    if (keyboardStates[SDL_SCANCODE_RIGHT])
        cam->rot_y += 0.04f;
}

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

