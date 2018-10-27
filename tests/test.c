#include <SDL.h>
#include <stdio.h>
#include <sqlite3.h>
#include <assert.h>

#define eprintf(msg, ...) fprintf(stderr, msg, __VA_ARGS__);

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;
    eprintf("%s", "example\n");

    return 0;
}
