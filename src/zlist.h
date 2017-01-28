#ifndef ZLIST_H
#define ZLIST_H

#include <stdlib.h>
#include "zdata.h"
#include "zexception.h"

/*
 * These methods are "internal," so to speak - they perform functions for the
 * macro with the corresponding name, and may behave unexpectedly if used by
 * themselves.
 */
extern void *zlist_add_(void*,size_t);
extern void *zlist_insert_(void*,size_t,size_t);
extern void zlist_delete_(void*,size_t,size_t);

extern void zlist_backstep(void*);
extern void zlist_rewind(void*,size_t);
extern void zlist_clear(void*);

#define zlist_of(type) type*

#define zlist_init(name,size)\
    (name = znew((size),sizeof(*name)))

#define zlist_add(name,value)\
    do {\
        (name)=zlist_add_((name),sizeof(*name);\
        (name)[zsize(name)-1]=(value))\
    } while(0)

#define zlist_insert(name,index,value)\
    do {\
        (name)=zlist_insert_((name),sizeof(*name),(index));\
        (name)[index]=(value);\
    } while(0)

#define zlist_delete(name,index)\
    (zlist_delete_(name,sizeof(*name),index))

#endif /* Header guard */
