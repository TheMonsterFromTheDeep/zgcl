#ifndef ZLIST_H
#define ZLIST_H

#include <stdlib.h>
#include <string.h>
#include "zexception.h"

/* Returns the size of the specified list. */
extern size_t zlist_size(void*);
/* Returns the number of preallocated elements for the specified list. */
extern size_t zlist_alloc(void*);
/* Frees the memory allocated for the specified list */
extern void zlist_free(void*);

/*  "Internal" methods - these are used by the macros to perform their various taks. */
extern void *zlist_new(size_t,size_t);
extern size_t zlist_index(void*,size_t);
extern void *zlist_fit(void*,size_t);
extern void *zlist_expand(void*,size_t,size_t);
extern size_t *zlist_size_ptr(void*);
extern size_t *zlist_alloc_ptr(void*);

#define zlist_of(type) type*
#define zlist_init(name,size) name = zlist_new(size,sizeof(*name))

#ifdef Z_NO_EXCEPT
    #define zlist_set(name,index,value) ((name)[(index)] = (value))
    #define zlist_get(name,index) ((name)[(index)])
    #define zlist_get_ptr(name,index) ((name) + (index))
#else
    #define zlist_set(name,index,value) ((name)[zlist_index((name),(index))] = (value))
    #define zlist_get(name,index) ((name)[zlist_index((name),(index))])
    #define zlist_get_ptr(name,index) ((name) + zlist_index((name),(index))])
#endif
#define zlist_add(name,value) do { \
    name = zlist_fit(name,sizeof(*name)); \
    name[zlist_size(name) - 1] = (value); \
} while(0)

#define zlist_removelast(name) (--(*zlist_size_ptr(name)))

#define zlist_addall(name,type,...) do { \
    type z_elems[] = { __VA_ARGS__ }; \
    size_t z_len = sizeof(z_elems) / sizeof(type); \
    size_t *z_list_len = zlist_size_ptr(name); \
    size_t z_i = 0; \
    name = zlist_expand(name,sizeof(*name),z_len); \
    for(; z_i < z_len; ++z_i) { \
        name[*z_list_len + z_i] = z_elems[z_i]; \
    } \
    *z_list_len += z_len; \
} while(0)

#endif
