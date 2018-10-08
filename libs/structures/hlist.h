/* Doubly linked lists with a single pointer list head
 * Implementation copied from the Linux Kernel
 */

#ifndef HLIST_H_INCLUDED
#define HLIST_H_INCLUDED

#ifndef ATTR
# if defined(NOATTRIBUTES)
#  define ATTR(X)
# else
#  define ATTR(X) __attribute__(X)
# endif /* NOATTRIBUTES */
#endif /* ATTR */

#ifndef container_of
# define container_of(ptr, type, member) __extension__({ \
        const __typeof__( ((type *)0)->member ) *__mptr = (ptr); \
        (type *)( (char *)__mptr - offsetof(type, member) );})
#if 0
# define container_of(ptr, type, member) \
    ((type *)(char *)(ptr) - offsetof(type, member))
#endif
#endif /* container_of */

#include <stddef.h>
#include <stdbool.h>

struct HList {
    struct HListNode *first;
};

struct HListNode {
    struct HListNode *next, **pprev;
};

#define HLIST_INIT { .first = NULL }

/** Declare and initialize an empty HList
 * @param name the name of the HList
 */
#define HLIST(name) \
    struct HList name = { .first = NULL }

/** Initialize an HList pointer
 * @param ptr the HList to initialize
 */
#define INIT_HLIST_PTR(ptr) \
    ((ptr)->first = NULL)

/** Initialize an HListNode pointer
 * @param h the HListNode initialize
 */
ATTR((nonnull(1)))
static inline void INIT_HLIST_NODE_PTR(struct HListNode *h)
{
    h->next = NULL;
    h->pprev = NULL;
}

/** Determine if the HListNode is hashed
 * @param n the HListNode to check
 */
ATTR((nonnull(1)))
static inline bool hlist_unhashed(const struct HListNode *n)
{
    return !n->pprev;
}

/** Determine if the HList is empty
 * @param h the HList to check
 */
ATTR((nonnull(1)))
static inline bool hlist_empty(const struct HList *h)
{
    return !h->first;
}

ATTR((nonnull(1)))
static inline void __hlist_del(struct HListNode *n)
{
    struct HListNode *next = n->next;
    struct HListNode **pprev = n->pprev;

    *pprev = next;
    if (next)
        next->pprev = pprev;
}

/** Delete \p n from its list
 * @param n the HListNode to delete
 */
ATTR((nonnull(1)))
static inline void hlist_del(struct HListNode *n)
{
    __hlist_del(n);
    n->next = NULL;
    n->pprev = NULL;
}

/** Delete \p n from its list and re-initialize it for reuse
 * @param n the HListNode to delete
 */
ATTR((nonnull(1)))
static inline void hlist_del_init(struct HListNode *n)
{
    if (!hlist_unhashed(n)) {
        __hlist_del(n);
        INIT_HLIST_NODE_PTR(n);
    }
}

/** Add \p n to \p h
 * @param n the HListNode to add
 * @param h the HList that is added to
 */
ATTR((nonnull(1, 2)))
static inline void hlist_add_head(struct HListNode *n, struct HList *h)
{
    struct HListNode *first = h->first;
    n->next = first;
    if (first)
        first->pprev = &n->next;
    h->first = n;
    n->pprev = &h->first;
}

/** Add \p n before \p next 
 * @param n the HListNode to add
 * @param next the HListNode that will be after \p n
 */
ATTR((nonnull(1, 2)))
static inline void hlist_add_before(struct HListNode *n,
                                    struct HListNode *next)
{
    n->pprev = next->pprev;
    n->next = next;
    next->pprev = &n->next;
    *(n->pprev) = n;
}

/** Add \p n after \p prev
 * @param n the HListNode to add
 * @param prev the HListNode that will be before \p n
 */
ATTR((nonnull(1, 2)))
static inline void hlist_add_behind(struct HListNode *n,
                                    struct HListNode *prev)
{
    n->next = prev->next;
    prev->next = n;
    n->pprev = &prev->next;

    if (n->next)
        n->next->pprev = &n->next;
}

/** Add an HListNode to a "fake" list
 * @param n the entry to add to a fake list
 */
ATTR((nonnull(1)))
static inline void hlist_add_fake(struct HListNode *n)
{
    n->pprev = &n->next;
}

/** Determine if a given HListNode is a fake
 * @param h the HListNode to test
 */
ATTR((nonnull(1)))
static inline bool hlist_fake(struct HListNode *h)
{
    return h->pprev == &h->next;
}

/** Determine if \p h is the one item in \p n
 * @param n the HListNode to check
 * @param h the HList to check
 */
ATTR((nonnull(1, 2)))
static inline bool hlist_is_singular_node(struct HListNode *n, 
                                          struct HList *h)
{
    return !n->next && n->pprev == &h->first;
}

/** Move a list from one HList to another
 * @param old the list to move from
 * @param new the list to move to
 */
ATTR((nonnull(1, 2)))
static inline void hlist_move_list(struct HList *old,
                                   struct HList *new)
{
    new->first = old->first;
    if (new->first)
        new->first->pprev = &new->first;
    old->first = NULL;
}

/** Get the structure for the given entry
 * @param ptr a pointer to the HList member
 * @param type the type of the structure you're looking for
 * @param member the name of the member that \p ptr is
 */
#define hlist_entry(ptr, type, member) \
    container_of(ptr, type, member)

/** Iterate over all the HListNodes \p pos in the HList \p head
 * @param pos the HListNode * to use as loop variable
 * @param head the HList * to iterate over
 */
#define hlist_for_each(pos, head) \
    for (pos = (head)->first; pos; pos = pos->next)

/** Iterate over all the HListNodes \p pos in the HList \p head safely
 * @param pos the HListNode * to use as loop variable
 * @param tmp a HListNode * to use as safety variable
 * @param head the HList * to iterate over
 */
/* #define hlist_for_each_safe(pos, tmp, head) \
    for (pos = (head)->first; \
            pos && __extension__({ tmp = pos->next; 1; }); \
            pos = tmp) */
#define hlist_for_each_safe(pos, tmp, head) \
    for (pos = (head)->first; pos && (tmp = pos->next, 1); pos = tmp)

/** Get the structure for the given entry, or NULL if \p ptr is NULL
 * @param ptr a pointer to the HList member
 * @param type the type of the structure you're looking for
 * @param member the name of the member that \p ptr is
 */
/* #define hlist_entry_safe(ptr, type, member)                \
    __extension__({                                        \
        __typeof__(ptr) ___ptr = (ptr);                    \
        ___ptr ? hlist_entry(___ptr, type, member) : NULL; \
    }) */
#define hlist_entry_safe(ptr, type, member) \
    (ptr ? hlist_entry(ptr, type, member) : NULL)

/** Iterate over an HList of a given type
 * @param pos the type * to use as loop variable
 * @param head the HList to iterate over
 * @param member the name of the HListNode within the structure
 */
#define hlist_for_each_entry(pos, head, member)                                  \
    for (pos = hlist_entry_safe((head)->first, __typeof__(*(pos)), member);      \
         pos;                                                                    \
         pos = hlist_entry_safe((pos)->member.next, __typeof__(*(pos)), member))

/** Resume iterating over an HList of a given type
 * @param pos the type * to use as loop variable, pointing to a current element
 * @param member the name of the HListNode within the structure
 */
#define hlist_for_each_entry_continue(pos, member)                               \
	for (pos = hlist_entry_safe((pos)->member.next, __typeof__(*(pos)), member); \
	     pos;                                                                    \
	     pos = hlist_entry_safe((pos)->member.next, __typeof__(*(pos)), member))

/** Iterate over an HList continuing from current point
 * @param pos the type * to use as a loop cursor
 * @param member the name of the HListNode within the struct
 */
#define hlist_for_each_entry_from(pos, member) \
	for (; pos; \
	     pos = hlist_entry_safe((pos)->member.next, __typeof__(*(pos)), member))

/** Iterate over list of given type safely
 * @param pos the type * to use as a loop cursor.
 * @param tmp another struct HListNode to use as safety variable
 * @param head the head for your list.
 * @param member the name of the HListNode within the struct.
 */
/* #define hlist_for_each_entry_safe(pos, tmp, head, member)                 \
    for (pos = hlist_entry_safe((head)->first, __typeof__(*pos), member); \
         pos && __extension__({ tmp = pos->member.next; 1; });            \
         pos = hlist_entry_safe(tmp, __typeof__(*pos), member)) */
#define hlist_for_each_entry_safe(pos, tmp, head, member)                 \
    for (pos = hlist_entry_safe((head)->first, __typeof__(*pos), member); \
         pos && (tmp = pos->member.next, 1);            \
         pos = hlist_entry_safe(tmp, __typeof__(*pos), member))

#endif
