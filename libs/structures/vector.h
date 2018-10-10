#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdlib.h>
#include <assert.h>

struct FloatVector {
    unsigned count;
    unsigned size;
    float *data;
};

struct UIntVector {
    unsigned count;
    unsigned size;
    unsigned *data;
};

#define VEC_INIT(varname, sz) \
{ .size = sz, .count = 0, .data = calloc(sz, sizeof (*(varname).data)) }

#define CHECK_SIZE(vec, padding) \
    if ((vec).count + (padding) > (vec).size) { \
        (vec).size *= 2; \
        expand_vector((void **)&(vec).data, (vec).size * sizeof (*(vec).data)); \
    }

#define VEC_NEXT(vec) \
    (vec).data[(vec).count++]

#define VEC_PREV(vec) \
    (vec).data[--(vec).count]

static inline void expand_vector(void ** vector, unsigned size)
{
    void *tmp = realloc(*vector, size);
    assert(tmp != NULL);
    *vector = tmp;
}

#endif
