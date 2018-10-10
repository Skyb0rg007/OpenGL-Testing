/* Statically sized hash table implementation
 * Taken from the Linux Kernel implementation
 */

#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

#include <stddef.h>
#include <stdbool.h>
#include "hash.h"
#include "hlist.h"

#ifndef ARRAY_SIZE
# define ARRAY_SIZE(arr) \
    (sizeof(arr) / sizeof((arr)[0]))
#endif

#define HASHTABLE(name, bits) \
    _Pragma("GCC warning \"Use DECLARE_HASHTABLE + hash_init instead\"") \
    struct HList name[1 << (bits)] = \
        { [0 ... ((1 << (bits)) - 1)] = HLIST_INIT }

#define DECLARE_HASHTABLE(name, bits) \
    struct HList name[1 << (bits)]

#define HASH_SIZE(name) \
    (ARRAY_SIZE(name))

#define HASH_BITS(name) \
    const_ilog2(HASH_SIZE(name))

#define hash_min(val, bits) \
    (sizeof (val) <= 4 ? hash_32(val, bits) : hash_long(val, bits))

ATTR((nonnull(1)))
static inline void __hash_init(struct HList *h, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        INIT_HLIST_PTR(&h[i]);
}

/** Initialize a hash table
 * @param hashtable hashtable to be initialized
 */
#define hash_init(hashtable) \
    __hash_init(hashtable, HASH_SIZE(hashtable))

/** Add an object to a hashtable
 * @param hashtable hashtable to add to
 * @param node the HListNode of the object to be added
 * @param key the key of the object to be added
 */
#define hash_add(hashtable, node, key) \
    hlist_add_head(node, &hashtable[hash_min(key, HASH_BITS(hashtable))])

/** Determine if an object is in any hashtable
 * @param node the HListNode of the object to be checked
 */
ATTR((nonnull(1)))
static inline bool hash_hashed(struct HListNode *node)
{
    return !hlist_unhashed(node);
}

ATTR((nonnull(1)))
static inline bool __hash_empty(struct HList *h, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        if (!hlist_empty(&h[i]))
            return false;

    return true;
}

/** Determine if a hashtable is empty
 * @param hashtable hashtable to check
 */
#define hash_empty(hashtable) __hash_empty(hashtable, HASH_SIZE(hashtable))

/** Remove an object from a hashtable
 * @param node the HListNode of the object to remove
 */
ATTR((nonnull(1)))
static inline void hash_del(struct HListNode *node)
{
    hlist_del_init(node);
}

/** Iterate over a hashtable
 * @param name hashtable to iterate over
 * @param bkt integer to use as the bucket loop variable
 * @param obj the type * to use as the entry loop variable
 * @param member the name of the HListNode within the struct
 */
#define hash_for_each(name, bkt, obj, member) \
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HASH_SIZE(name); (bkt)++) \
        hlist_for_each_entry(obj, &name[bkt], member)

/** Iterate over a hashtable safely
 * @param name hashtable to iterate over
 * @param bkt integer to use as the bucket loop variable
 * @param tmp a HListNode * used for a temporary variable
 * @param obj the type * to use as the entry loop variable
 * @param member the name of the HListNode within the struct
 */
#define hash_for_each_safe(name, bkt, tmp, obj, member) \
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HASH_SIZE(name); (bkt)++) \
        hlist_for_each_entry_safe(obj, tmp, &name[bkt], member)

/** Iterate over all possible object hashing to the same bucket
 * @param name hashtable to iterate over
 * @param obj the type * to use as a loop variable for each entry
 * @param member the name of the HListNode within the struct
 * @param key the key of the object to iterate over
 */
#define hash_for_each_possible(name, obj, member, key)			\
	hlist_for_each_entry(obj, &name[hash_min(key, HASH_BITS(name))], member)


/** Iterate over all possible object hashing to the same bucket safely
 * @param name hashtable to iterate over
 * @param obj the type * to use as a loop variable for each entry
 * @param tmp a type * to se as a temporary variable
 * @param member the name of the HListNode within the struct
 * @param key the key of the object to iterate over
 */
#define hash_for_each_possible_safe(name, obj, member, key)			\
	hlist_for_each_entry_safe(obj, tmp, \
            &name[hash_min(key, HASH_BITS(name))], member)

#ifndef const_ilog2
# define const_ilog2(n)             \
(                                   \
	__builtin_constant_p(n) ? (     \
		(n) < 2 ? 0 :               \
		(n) & (1ULL << 63) ? 63 :   \
		(n) & (1ULL << 62) ? 62 :   \
		(n) & (1ULL << 61) ? 61 :   \
		(n) & (1ULL << 60) ? 60 :   \
		(n) & (1ULL << 59) ? 59 :   \
		(n) & (1ULL << 58) ? 58 :   \
		(n) & (1ULL << 57) ? 57 :   \
		(n) & (1ULL << 56) ? 56 :   \
		(n) & (1ULL << 55) ? 55 :   \
		(n) & (1ULL << 54) ? 54 :   \
		(n) & (1ULL << 53) ? 53 :   \
		(n) & (1ULL << 52) ? 52 :   \
		(n) & (1ULL << 51) ? 51 :   \
		(n) & (1ULL << 50) ? 50 :   \
		(n) & (1ULL << 49) ? 49 :   \
		(n) & (1ULL << 48) ? 48 :   \
		(n) & (1ULL << 47) ? 47 :   \
		(n) & (1ULL << 46) ? 46 :   \
		(n) & (1ULL << 45) ? 45 :   \
		(n) & (1ULL << 44) ? 44 :   \
		(n) & (1ULL << 43) ? 43 :   \
		(n) & (1ULL << 42) ? 42 :   \
		(n) & (1ULL << 41) ? 41 :   \
		(n) & (1ULL << 40) ? 40 :   \
		(n) & (1ULL << 39) ? 39 :   \
		(n) & (1ULL << 38) ? 38 :   \
		(n) & (1ULL << 37) ? 37 :   \
		(n) & (1ULL << 36) ? 36 :   \
		(n) & (1ULL << 35) ? 35 :   \
		(n) & (1ULL << 34) ? 34 :   \
		(n) & (1ULL << 33) ? 33 :   \
		(n) & (1ULL << 32) ? 32 :   \
		(n) & (1ULL << 31) ? 31 :   \
		(n) & (1ULL << 30) ? 30 :   \
		(n) & (1ULL << 29) ? 29 :   \
		(n) & (1ULL << 28) ? 28 :   \
		(n) & (1ULL << 27) ? 27 :   \
		(n) & (1ULL << 26) ? 26 :   \
		(n) & (1ULL << 25) ? 25 :   \
		(n) & (1ULL << 24) ? 24 :   \
		(n) & (1ULL << 23) ? 23 :   \
		(n) & (1ULL << 22) ? 22 :   \
		(n) & (1ULL << 21) ? 21 :   \
		(n) & (1ULL << 20) ? 20 :   \
		(n) & (1ULL << 19) ? 19 :   \
		(n) & (1ULL << 18) ? 18 :   \
		(n) & (1ULL << 17) ? 17 :   \
		(n) & (1ULL << 16) ? 16 :   \
		(n) & (1ULL << 15) ? 15 :   \
		(n) & (1ULL << 14) ? 14 :   \
		(n) & (1ULL << 13) ? 13 :   \
		(n) & (1ULL << 12) ? 12 :   \
		(n) & (1ULL << 11) ? 11 :   \
		(n) & (1ULL << 10) ? 10 :   \
		(n) & (1ULL <<  9) ?  9 :   \
		(n) & (1ULL <<  8) ?  8 :   \
		(n) & (1ULL <<  7) ?  7 :   \
		(n) & (1ULL <<  6) ?  6 :   \
		(n) & (1ULL <<  5) ?  5 :   \
		(n) & (1ULL <<  4) ?  4 :   \
		(n) & (1ULL <<  3) ?  3 :   \
		(n) & (1ULL <<  2) ?  2 :   \
		1) :                        \
	-1)
#endif /* const_ilog2 */

#endif
