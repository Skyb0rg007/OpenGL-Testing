#define SDL_ASSERT_LEVEL 3
#include <SDL2/SDL.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_assert("test:" && 1 == 0);
    sleep(1);
    SDL_Quit();
    return 0;
}
