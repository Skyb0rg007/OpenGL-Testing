#include "ECS.h"
#include "mainloop.h"

void update_world(World *world, const system_func *sys, size_t sys_ct, 
        const system_func_const *const_sys, size_t const_sys_ct)
{
    for (size_t i = 0; i < sys_ct; i++) {
        sys[i](world);
    }
    for (size_t i = 0; i < const_sys_ct; i++) {
        const_sys[i](world);
    }
}
