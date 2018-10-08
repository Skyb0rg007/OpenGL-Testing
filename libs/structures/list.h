/* Implementation of a doubly linked list
 * Copied from the Linux Kernel implementation
 */

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stddef.h>
#include <stdbool.h>

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
#endif /* container_of */

struct List {
    struct List *next,
                    *prev;
};

#define LIST_INIT(name) { &(name), &(name) }

/** Declare and initialize an empty List 
 * @param name the name of the List
 */
#define LIST(name) \
    struct List name = LIST_INIT(name)

/** Initialize an empty List 
 * @param list List to initialize
 */
ATTR((nonnull(1)))
static inline void INIT_LIST_PTR(struct List *list)
{
    list->next = list;
    list->prev = list;
}

/** Insert an entry between two known consecutive entries
 * @param new the List to insert
 * @param prev the List before
 * @param next the List after
 */
ATTR((nonnull(1, 2)))
static inline void __list_add(struct List *new, struct List *prev, 
        struct List *next)
{
    next->prev = new;
    new->next  = next;
    new->prev  = prev;
    prev->next = new;
}

/** Insert an entry after head
 * @param new the List to insert
 * @param head the List to add the new item to
 */
ATTR((nonnull(1, 2)))
static inline void list_add(struct List *new, struct List *head)
{
    __list_add(new, head, head->next);
}

/** Insert an entry before the head
 * @param new the List to insert
 * @param head the List to add the new item to
 */
ATTR((nonnull(1, 2)))
static inline void list_add_tail(struct List *new, struct List *head)
{
    __list_add(new, head->prev, head);
}

/** Delete list entry between two consecutive entries
 * @param prev the List before
 * @param next the List after
 */
ATTR((nonnull(1, 2)))
static inline void __list_del(struct List *prev, struct List *next)
{
    next->prev = prev;
    prev->next = next;
}

/** Delete \p entry from the list
 * @param entry the List to delete
 */
ATTR((nonnull(1)))
static inline void __list_del_entry(struct List *entry)
{
    __list_del(entry->prev, entry->next);
}

/** Delete \p entry from the list and set next and prev to NULL
 * @param entry the List to delete
 */
ATTR((nonnull(1)))
static inline void list_del(struct List *entry)
{
    __list_del_entry(entry);
    entry->next = NULL;
    entry->prev = NULL;
}

/** Delete \p entry from the list and re-initialize
 * @param entry the List to delete
 */
ATTR((nonnull(1)))
static inline void list_del_init(struct List *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_PTR(entry);
}

/** Replace \p old with \p new
 * @param old the List to replace
 * @param new the List to replace \p old with
 */
ATTR((nonnull(1, 2)))
static inline void list_replace(struct List *old, struct List *new)
{
    new->next       = old->next;
    new->next->prev = new;
    new->prev       = old->prev;
    new->prev->next = new;
}

/** Replace \p old with \p new and re-initialize \p old for re-use
 * @param old the List to replace
 * @param new the List to replace \p old with
 */
ATTR((nonnull(1, 2)))
static inline void list_replace_init(struct List *old, struct List *new)
{
    list_replace(old, new);
    INIT_LIST_PTR(old);
}

/** Move \p list from its list to the start of \p head
 * @param list the List to move
 * @param head where to move \p list to
 */
ATTR((nonnull(1, 2)))
static inline void list_move(struct List *list, struct List *head)
{
    __list_del_entry(list);
    list_add(list, head);    
}

/** Move \p list from its list to the end of \p head
 * @param list the List to move
 * @param head where to move \p list to
 */
ATTR((nonnull(1, 2)))
static inline void list_move_tail(struct List *list, struct List *head)
{
    __list_del_entry(list);
    list_add_tail(list, head);
}

/** Determine if \p list is the last item in \p head
 * @param list the element to check
 * @param head the List to check
 */
ATTR((nonnull(1, 2)))
static inline bool list_is_last(const struct List *list, 
        const struct List *head)
{
    return list->next == head;
}

/** Determine if \p head is empty
 * @param head the List to check
 */
ATTR((nonnull(1)))
static inline bool list_empty(const struct List *head)
{
    return head->next == head;
}

/** Rotate \p head so the first element becomes the last
 * @param head the List to rotate
 */
ATTR((nonnull(1)))
static inline void list_rotate_left(struct List *head)
{
    if (!list_empty(head)) {
        struct List *first = head->next;
        list_move_tail(first, head);
    }
}

/** Determine if \p head contains only one element
 * @param head the List to check
 */
ATTR((nonnull(1)))
static inline bool list_is_singular(const struct List *head)
{
    return !list_empty(head) && (head->next == head->prev);
}

ATTR((nonnull(1, 2, 3)))
static inline void __list_cut_position(struct List *list,
        struct List *head,
        struct List *entry)
{
    struct List *new_first = entry->next;

    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    list->prev->next = list;

    head->next = new_first;
    new_first->prev = head;
}

/** Cut a list into two
 * @param list new list to add removed entries to (empty list)
 * @param head a list with entries to cut
 * @param entry an entry within \p head, could be head itself
 *
 * Moved the initial part of \p head, up to and including \p entry,
 * from \p head to \p list
 */
ATTR((nonnull(1, 2, 3)))
static inline void list_cut_position(struct List *list,
        struct List *head, struct List *entry)
{
    if (list_empty(head))
        return;
    if (list_is_singular(head) &&
            (head->next != entry && head != entry))
        return;
    if (entry == head)
        INIT_LIST_PTR(list);
    else
        __list_cut_position(list, head, entry);
}

ATTR((nonnull(1, 2, 3)))
static inline void __list_splice(const struct List *list,
        struct List *prev, struct List *next)
{
    struct List *first = list->next;
    struct List *last = list->prev;

    first->prev = prev;
    prev->next  = first;

    last->next = next;
    next->prev = last;
}

/** Join two lists together
 * @param list the List to add
 * @param head the List to add \p list after
 */
static inline void list_splice(const struct List *list, struct List *head)
{
    if (!list_empty(list))
        __list_splice(list, head, head->next);
}

/** Join two lists together
 * @param list the List to add
 * @param head the List to add \p list before 
 */
ATTR((nonnull(1, 2)))
static inline void list_splice_tail(const struct List *list,
        struct List *head)
{
    if (!list_empty(list))
        __list_splice(list, head->prev, head);
}

/** Join \p list with \p head, re-initializing \p list
 * @param list the List to add to \p head
 * @param head the List to be added to
 */
ATTR((nonnull(1, 2)))
static inline void list_splice_init(struct List *list, struct List *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
        INIT_LIST_PTR(list);
    }
}

/** Join \p list with \p head, re-initializing \p list
 * @param list the List to add to \p head
 * @param head the List to be added to
 */
static inline void list_splice_tail_init(struct List *list,
        struct List *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head->prev, head);
        INIT_LIST_PTR(list);
    }
}

/* --- Macro Magic --- */

/** Get the structure for the given entry
 * @param ptr a pointer to the List member
 * @param type the type of the structure you're looking for
 * @param member the name of the member that \p ptr is
 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

/** Get the first element from a (non-empty) list 
 * @param ptr the List structure (struct List *)
 * @param type the type of the list element
 * @param member the name of the List within the struct
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

/** Get the last element from a (non-empty) list 
 * @param ptr the List structure (struct List *)
 * @param type the type of the list element
 * @param member the name of the List within the struct
 */
#define list_last_entry(ptr, type, member) \
    list_entry((ptr)->prev, type, member)

/** Get the first element from a list, or NULL if list is empty 
 * @param ptr the List structure (struct List *)
 * @param type the type of the list element
 * @param member the name of the List within the struct
 */
#define list_first_entry_or_null(ptr, type, member) __extension__ ({ \
        struct List *head__ = (ptr); \
        struct List *pos__ = READ_ONCE(head__->next); \
        pos__ != head__ ? \
        list_entry(pos__, type, member) \
        : NULL; \
        })

/** Get next element in list 
 * @param pos the current list entry
 * @param member the name of the List struct in the entry
 */
#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, __typeof__(*(pos)), member)

/** Get previous element in list 
 * @param pos the current list entry
 * @param member the name of the List struct in the entry
 */
#define list_prev_entry(pos, member) \
    list_entry((pos)->member.prev, __typeof__(*(pos)), member)

/* For loops: */

/** Iterate over the `struct List` items 
 * @param pos (struct List *) to use as loop variable
 * @param head the List to iterate over
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/** Iterate over the `struct List` items backwards
 * @param pos (struct List *) to use as loop variable
 * @param head the List to iterate over
 */
#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

/** Iterate over the `struct List` items 
 * @param pos (struct List *) to use as loop variable
 * @param tmp (struct List *) to use as safety variable
 * @param head the List to iterate over
 */
#define list_for_each_safe(pos, tmp, head) \
    for (pos = (head)->next, tmp = pos->next; \
            pos != (head); \
            pos = tmp, tmp = pos->next)

/** Iterate over the `struct List` items backwards
 * @param pos (struct List *) to use as loop variable
 * @param tmp (struct List *) to use as safety variable
 * @param head the List to iterate over
 */
#define list_for_each_prev_safe(pos, tmp, head) \
    for (pos = (head)->prev, tmp = pos->prev; \
            pos != (head); \
            pos = tmp, tmp = pos->prev)


/** Iterate with the struct that the List is embedded in 
 * @param pos the structure pointer to use as loop variable
 * @param head the List to iterate over
 * @param member the name of List within the structure
 */
#define list_for_each_entry(pos, head, member) \
    for (pos = list_first_entry(head, __typeof__(*pos), member); \
            &pos->member != (head); \
            pos = list_next_entry(pos, member))

/** Iterate with the struct that the List is embedded in backwards
 * @param pos the structure pointer to use as loop variable
 * @param head the List to iterate over
 * @param member the name of List within the structure
 */
#define list_for_each_entry_reverse(pos, head, member) \
    for (pos = list_last_entry(head, __typeof__(*pos), member); \
            &pos->member != (head); \
            pos = list_prev_entry(pos, member))

/** Prepare \p pos for use in list_for_each_entry_continue() 
 * @param pos the type * to use as a starting point
 * @param head the List to iterate over
 * @param member the name of the List within the structure 
 */
#define list_prepare_entry(pos, head, member) \
    ((pos) ? : list_entry(head, __typeof__(*pos), member))

/** Continue to iterate over a List
 * @param pos the type * to use as a loop variable
 * @param head the List to iterate over
 * @param member the name of the List within the structure
 */
#define list_for_each_entry_continue(pos, head, member) 		\
    for (pos = list_next_entry(pos, member);			\
            &pos->member != (head);					\
            pos = list_next_entry(pos, member))

/** Continue to iterate over a List backwards
 * @param pos the type * to use as a loop variable
 * @param head the List to iterate over
 * @param member the name of the List within the structure
 */
#define list_for_each_entry_continue_reverse(pos, head, member)		\
    for (pos = list_prev_entry(pos, member);			\
            &pos->member != (head);					\
            pos = list_prev_entry(pos, member))

/** Iterate over the List, starting with position \p pos
 * @param pos pointer to a member of the \p head list, loop variable
 * @param head the List to iterate over
 * @param member the name of the List within the structure
 */
#define list_for_each_entry_from(pos, head, member) 			\
    for (; &pos->member != (head);					\
            pos = list_next_entry(pos, member))

/** Iterate over the List, starting with position \p pos backwards
 * @param pos pointer to a member of the \p head list, loop variable
 * @param head the List to iterate over
 * @param member the name of the List within the structure
 */
#define list_for_each_entry_from_reverse(pos, head, member)		\
    for (; &pos->member != (head);					\
            pos = list_prev_entry(pos, member))

/** Iterate over the List safely
 * @param pos structure pointer to use as loop variable
 * @param n structure pointer to use as safety variable, same type as \p pos
 * @param head the List to iterate over
 * @param member the name of the List within the structure
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
    for (pos = list_first_entry(head, __typeof__(*pos), member),	\
            n = list_next_entry(pos, member);			\
            &pos->member != (head); 					\
            pos = n, n = list_next_entry(n, member))

/** Continue to iterate over a List safely
 * @param pos the type * to use as a loop variable
 * @param head the List to iterate over
 * @param member the name of the List within the structure
 */
#define list_for_each_entry_safe_continue(pos, n, head, member) 		\
    for (pos = list_next_entry(pos, member), 				\
            n = list_next_entry(pos, member);				\
            &pos->member != (head);						\
            pos = n, n = list_next_entry(n, member))

/** Iterate over the List, starting with position \p pos
 * @param pos pointer to a member of the \p head list, loop variable
 * @param n structure pointer to use as safety variable, same type as \p pos
 * @param head the List to iterate over
 * @param member the name of the List within the structure
 */
#define list_for_each_entry_safe_from(pos, n, head, member) 			\
    for (n = list_next_entry(pos, member);					\
            &pos->member != (head);						\
            pos = n, n = list_next_entry(n, member))

/** Iterate over the List backwards, safely
 * @param pos structure pointer to use as loop variable
 * @param n structure pointer to use as safety variable, same type as \p pos
 * @param head the List to iterate over
 * @param member the name of the List within the structure
 */
#define list_for_each_entry_safe_reverse(pos, n, head, member)		\
    for (pos = list_last_entry(head, __typeof__(*pos), member),		\
            n = list_prev_entry(pos, member);			\
            &pos->member != (head); 					\
            pos = n, n = list_prev_entry(n, member))

/** Reset a stale list_for_each_entry_safe loop
 * @param pos the loop variable used in the list_for_each_entry_safe loop
 * @param n	safety variable used in list_for_each_entry_safe
 * @param member the name of the List within the structure
 */
#define list_safe_reset_next(pos, n, member)				\
	n = list_next_entry(pos, member)

#endif
