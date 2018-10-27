#ifndef MAINLOOP_H_INCLUDED
#define MAINLOOP_H_INCLUDED

#include "ECS.h"

void update_world(World *, const system_func *systems, size_t count, 
        const system_func_const *const_systems, size_t const_count);

#endif
