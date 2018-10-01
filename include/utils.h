#include <GL/glew.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(NDEBUG)

# define $glcheck(call) call
# define $log(...)    (void)0
# define $fatal(...)  (void)0
# define $assert(...) (void)0

#else

#define $log(...) do {                                 \
    fprintf(stderr, "%s[%u]: \n", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                      \
    fprintf(stderr, "\n");                             \
} while (0)

#define $fatal(...) do {                               \
    $log(__VA_ARGS__);                                 \
    exit(EXIT_FAILURE);                                \
} while (0)

#define $assert(X, MSG) do {                           \
    if (!(X))                                          \
        $fatal("%s", MSG);                             \
} while (0)


#define $glcheck(call) do {                            \
    call;                                              \
    GLenum gl_err;                                     \
    bool gl_had_error = false;                         \
    while ((gl_err = glGetError()) != GL_NO_ERROR) {   \
        $log("%s\nGL error 0x%x: %s",                  \
            #call, gl_err, glEnumName(gl_err));        \
        gl_had_error = true;                           \
    }                                                  \
    if (gl_had_error)                                  \
        exit(EXIT_FAILURE);                            \
} while (0)
#endif

#define ARRAY_SIZE(arr) \
            sizeof(arr) / sizeof((arr)[0])

static inline const char *glEnumName(GLenum e)
{
    switch(e)
    {
        case GL_TEXTURE:
            return "GL_TEXTURE";
        case GL_RENDERBUFFER:
            return "GL_RENDERBUFFER";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        case GL_FRAMEBUFFER_UNSUPPORTED:
            return "GL_FRAMEBUFFER_UNSUPPORTED";
        default:
            return "(unknown)";
    }
}
