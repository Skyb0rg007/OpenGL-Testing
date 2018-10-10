#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "vector.h"
#include "hashtable.h"

int main(int argc, char *argv[])
{
    struct FloatVector vec = VEC_INIT(vec, 4);

    for (unsigned i = 0; i < 20; i++) {
        fprintf(stderr, "{ .size = %2u, .count = %2u }\n", vec.size, vec.count);
        CHECK_SIZE(vec, 1);
        VEC_NEXT(vec) = (float)i;
    }

    for (unsigned i = 0; vec.count != 0; i++) {
        printf("vec[%2u] = %2.1f\n", i, VEC_PREV(vec));
    }

    free(vec.data);
    return 0;
}
