#ifndef ZLIST_H
#define ZLIST_H

#include <stdlib.h>
#include <string.h>

size_t zlist_size(void*);
size_t zlist_alloc(void*);
void *zlist_new(size_t,size_t);
int zlist_validate(void*,size_t);
void *zlist_fit(void*,size_t);
void zlist_free(void*);

#define zlist_of(type) type*
#define zlist_init(name,size) name = zlist_new(size,sizeof(*name))

#ifdef Z_NO_EXCEPT
    #define zlist_set(name,index,value) ((name)[(index)] = (value))
    #define zlist_get(name,index) ((name)[(index)])
#else
    //#define zlist_set(name,index,value) do { zlist_validate(name,index); name[index] = (value); } while(0)
    #define zlist_set(name,index,value) do { void *z_ptr = (name); memcpy(z_ptr + sizeof(*name) * index, &value, sizeof(*name)); } while(0);
    // => zlist_memcpy(z_ptr, (index), (value))
    //#define zlist_get(name,index) (void *ptr = (name))[(index)(void
    #define zlist_get(name,index) (zlist_validate(name,index) ? name[index] : *name)
    // => ((name)[zlist_validate((name),(index))])
    #define zlist_get_ptr(name,index) (zlist_validate((name),(index)) ? ((name) + (index)) : (name))
#endif
#define zlist_add(name,value) do { name = zlist_fit(name,sizeof(*name)); name[zlist_size(name) - 1] = (value); } while(0)
#define zlist_addall(name,type,...) do { type z_elems[] = { __VA_ARGS__ }; size_t z_len = sizeof(z_elems) / sizeof(z_elems[0]); size_t z_i = 0; for(; z_i < z_len; ++z_i) { zlist_add(name,z_elems[z_i]); } } while(0)

#endif
