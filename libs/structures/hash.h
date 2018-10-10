/* Hash implementation
 * Taken from the Linux Kernel implementation
 */

#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#ifndef ATTR
# if defined(NOATTRIBUTES)
#  define ATTR(X)
# else
#  define ATTR(X) __attribute__(X)
# endif
#endif

#if __LP64__
/* 64 bit */
# define GOLDEN_RATIO_PRIME GOLDEN_RATIO_64
# define hash_long(val, bits) hash_64(val, bits)
#else
/* 32 bit */
# define GOLDEN_RATIO_PRIME GOLDEN_RATIO_32
# define hash_long(val, bits) hash_32(val, bits)
#endif /* __LP64__ */

#define GOLDEN_RATIO_32 0x61C88647
#define GOLDEN_RATIO_64 0x61C8864680B583EBull

static inline uint32_t hash_32(uint32_t val, unsigned int bits)
{
    return (val * GOLDEN_RATIO_32) >> (32 - bits);
}

static inline uint32_t hash_64(uint64_t val, unsigned int bits)
{
#if __LP64__ /* 64 bit */
    return val * GOLDEN_RATIO_64 >> (64 - bits);
#else
    return hash_32((uint32_t)val ^ ((val >> 32) * GOLDEN_RATIO_32), bits);
#endif
}

/* static inline uint32_t hash_ptr(const void *ptr, unsigned int bits) */
/* { */
    /* return hash_long((unsigned long)ptr, bits); */
/* } */

/* // Should be called fold32_ptr */
/* static inline uint32_t hash32_ptr(const void *ptr) */
/* { */
    /* unsigned long val = (unsigned long)ptr; */
/* #if __LP64__ [> 64 bit <] */
    /* val ^= (val >> 32); */
/* #endif */
    /* return (uint32_t)val; */
/* } */

#define FNV_PRIME    16777619
#define OFFSET_BASIS 2166136261

ATTR((nonnull(1)))
static inline uint32_t hash_fnv(const char *str)
{
    uint32_t hash = OFFSET_BASIS;
    for (size_t i = 0; str[i] != '\0'; i++) {
        hash ^= str[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

#endif /* HASH_H_INCLUDED */
