/* utils.h - Useful program utility macros
 *
 * Macros for debugging and basic program usage
 */
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <GL/glew.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* Convenience Macros */
#define ARRAY_SIZE(arr) sizeof(arr) / sizeof((arr)[0])
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

# define LOG(...)     (void)0
# define FATAL(...)   exit(EXIT_FAILURE)
# define ASSERT(X, Y) (void)sizeof(X) /* Prevents unused warnings */

#else /* NDEBUG */

/* Disable calling SDL_Quit() when debugging */
# define SDL_Quit() (void)0

# define LOG(...) do {                                 \
    fprintf(stderr, "%s[%u]: \n", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                      \
    fprintf(stderr, "\n");                             \
} while (0)

# define FATAL(...) do {                               \
    LOG(__VA_ARGS__);                                  \
    exit(EXIT_FAILURE);                                \
} while (0)

# define ASSERT(X, MSG) do {                           \
    if (!(X))                                          \
        FATAL("%s", MSG);                              \
} while (0)

#endif /* NDEBUG */

#endif /* UTILS_H_INCLUDED */
