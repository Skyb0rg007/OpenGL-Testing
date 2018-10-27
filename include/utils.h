/* utils.h - Useful program utility macros
 *
 * Macros for debugging and basic program usage
 */
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

/* General helper functions */
char *load_file(const char *path);
long my_getline(char **lineptr, size_t *n, FILE *stream);
long my_getdelim(char **lineptr, size_t *n, int delim, FILE *stream);
float tofloat(const char *str);
unsigned touint(const char *str);

/* Convenience Macros */
#define ARRAY_SIZE(arr) sizeof (arr) / sizeof ((arr)[0])
#define ABS(x) ((x) < 0 ? ((-x)) : (x))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define PI 3.14159265359
#define DEGREES(rad) ((rad) * 180.0 / PI)
#define RADIANS(deg) ((deg) * PI / 180.0)

/* Disable attributes if the compiler doesn't support them */
#if defined(NOATTRIBUTES)
# define ATTR(X)
#else
# define ATTR(X) __attribute__(X)
#endif /* NOATTRIBUTES */

/* Logging + Error-checking macros */
#if defined(NDEBUG)
/* NOTE: sizeof is used to prevent 'unused variable' warnings */
# define LOG(...)        (void)sizeof ((__VA_ARGS__))
# define FATAL(...)      exit(EXIT_FAILURE)
# define ASSERT(X, ...)  (void)sizeof (X)

#else /* NDEBUG */

/* Disable calling SDL_Quit() when debugging */
# define SDL_Quit() (void)0

# define LOG(...) do {                                 \
    fprintf(stderr, "%s[%u]: \n", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                      \
    fprintf(stderr, "\n");                             \
} while (0)

# define FATAL(...) do {                               \
    /* TODO: This doesn't work on MVSC */              \
    LOG(__VA_ARGS__);                                  \
    exit(EXIT_FAILURE);                                \
} while (0)

# define ASSERT(X, ...) do {                           \
    if (!(X))                                          \
        FATAL(__VA_ARGS__);                            \
} while (0)

#endif /* NDEBUG */

#endif /* UTILS_H_INCLUDED */
