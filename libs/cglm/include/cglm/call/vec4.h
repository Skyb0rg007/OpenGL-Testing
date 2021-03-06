/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef cglmc_vec4_h
#define cglmc_vec4_h
#ifdef __cplusplus
extern "C" {
#endif

#include "../cglm.h"

/* DEPRECATED! use _copy, _ucopy versions */
#define glmc_vec4_dup3(v, dest) glmc_vec4_copy3(v, dest)
#define glmc_vec4_dup(v, dest)  glmc_vec4_copy(v, dest)

CGLM_EXPORT
void
glmc_vec4(vec3 v3, float last, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_zero(vec4 v);

CGLM_EXPORT
void
glmc_vec4_one(vec4 v);

CGLM_EXPORT
void
glmc_vec4_copy3(vec4 a, vec3 dest);

CGLM_EXPORT
void
glmc_vec4_copy(vec4 v, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_ucopy(vec4 v, vec4 dest);

CGLM_EXPORT
float
glmc_vec4_dot(vec4 a, vec4 b);

CGLM_EXPORT
float
glmc_vec4_norm(vec4 vec);

CGLM_EXPORT
float
glmc_vec4_norm2(vec4 vec);

CGLM_EXPORT
void
glmc_vec4_normalize_to(vec4 vec, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_normalize(vec4 v);

CGLM_EXPORT
void
glmc_vec4_add(vec4 a, vec4 b, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_adds(vec4 v, float s, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_sub(vec4 a, vec4 b, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_subs(vec4 v, float s, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_mul(vec4 a, vec4 b, vec4 d);

CGLM_EXPORT
void
glmc_vec4_scale(vec4 v, float s, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_scale_as(vec3 v, float s, vec3 dest);

CGLM_EXPORT
void
glmc_vec4_div(vec4 a, vec4 b, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_divs(vec4 v, float s, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_addadd(vec4 a, vec4 b, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_subadd(vec4 a, vec4 b, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_muladd(vec4 a, vec4 b, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_muladds(vec4 a, float s, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_flipsign(vec4 v);

CGLM_EXPORT
void
glmc_vec4_flipsign_to(vec4 v, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_inv(vec4 v);

CGLM_EXPORT
void
glmc_vec4_inv_to(vec4 v, vec4 dest);

CGLM_EXPORT
float
glmc_vec4_distance(vec4 v1, vec4 v2);

CGLM_EXPORT
void
glmc_vec4_maxv(vec4 v1, vec4 v2, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_minv(vec4 v1, vec4 v2, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_clamp(vec4 v, float minVal, float maxVal);

CGLM_EXPORT
void
glmc_vec4_lerp(vec4 from, vec4 to, float t, vec4 dest);

/* ext */

CGLM_EXPORT
void
glmc_vec4_mulv(vec4 a, vec4 b, vec4 d);

CGLM_EXPORT
void
glmc_vec4_broadcast(float val, vec4 d);

CGLM_EXPORT
bool
glmc_vec4_eq(vec4 v, float val);

CGLM_EXPORT
bool
glmc_vec4_eq_eps(vec4 v, float val);

CGLM_EXPORT
bool
glmc_vec4_eq_all(vec4 v);

CGLM_EXPORT
bool
glmc_vec4_eqv(vec4 v1, vec4 v2);

CGLM_EXPORT
bool
glmc_vec4_eqv_eps(vec4 v1, vec4 v2);

CGLM_EXPORT
float
glmc_vec4_max(vec4 v);

CGLM_EXPORT
float
glmc_vec4_min(vec4 v);

CGLM_EXPORT
bool
glmc_vec4_isnan(vec4 v);

CGLM_EXPORT
bool
glmc_vec4_isinf(vec4 v);

CGLM_EXPORT
bool
glmc_vec4_isvalid(vec4 v);

CGLM_EXPORT
void
glmc_vec4_sign(vec4 v, vec4 dest);

CGLM_EXPORT
void
glmc_vec4_sqrt(vec4 v, vec4 dest);

#ifdef __cplusplus
}
#endif
#endif /* cglmc_vec4_h */

