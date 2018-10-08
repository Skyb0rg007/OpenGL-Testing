#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "hashtable.h"

struct Element {
    const char *key;
    int value;
    struct HListNode hh;
};

int main(int argc, char *argv[])
{
    DECLARE_HASHTABLE(hashtable, 3);
    hash_init(hashtable);
    for (int i = 1; i < argc; i++) {
        struct Element *e = malloc(sizeof *e);
        e->key = argv[i];
        e->value = i;
        INIT_HLIST_NODE_PTR(&e->hh);
        hash_add(hashtable, &e->hh, hash_fnv(e->key));
    }

    char str[50];
    printf("Enter a key to search for: ");
    fgets(str, sizeof str, stdin);
    str[strlen(str)-1] = '\0';

    printf("\n\nSearching for item with key = \"%s\" (hash = %"PRIu32")\n", 
            str, hash_fnv(str));

    struct Element *e;
    hash_for_each_possible(hashtable, e, hh, hash_fnv(str))
    {
        printf("Checking { .key = \"%s\", .value = %2d }\n", e->key, e->value);
        if (strcmp(e->key, str) == 0) {
            printf("Element confirmed!\n");
            break;
        }
    }

    unsigned int bucket;
    struct HListNode *tmp;
    hash_for_each_safe(hashtable, bucket, tmp, e, hh)
    {
        printf("freeing { .key = \"%s\", .value = %2d } (hash = %"PRIu32")\n", 
                e->key, e->value, hash_fnv(e->key));
        free(e);
    }
    
    return 0;
}
