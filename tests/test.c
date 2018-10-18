#include <SDL.h>
#include <stdio.h>
#include <sqlite3.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;
    char *str = SDL_GetPrefPath("org.name", "test-app");
    printf("PrefPath = %s\n", str);
    SDL_free(str);

    return 0;
}
