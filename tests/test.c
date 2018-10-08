#include <SDL.h>
#include <sqlite3.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

    sqlite3 *db;
    int ret = sqlite3_open(":memory:", &db);
    assert(ret == SQLITE_OK);
    ret = sqlite3_close(db);
    assert(ret == SQLITE_OK);

    SDL_Log("This is a log message");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Debug message");
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error message");
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Critical message");
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, "Debug message");

    return 0;
}
