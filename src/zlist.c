#include <stdlib.h>

#include "zdata.h"
#include "zexception.h"
#include "zlist.h"
#include "zbase.h"

static void zlist_expand(void **list, size_t elemsize) {
    if(zsize(*list) + 1 > zalloc(*list)) {
        void *tmp = zrealloct(*list, 1 + zalloc(*list) * 2, elemsize);
        if(!tmp) {
            throw("Error while reallocating list!");
            return 0;
        }
        *list = tmp;
    }
}

void *zlist_add_(void *list, size_t elemsize) {
    if(!list) { return NULL; }

    zlist_expand(&list, elemsize);

    ++*zsizeptr(list);

    return list;
}

void *zlist_insert_(void *list, size_t elemsize, size_t index) {
    if(!list) { return NULL; }

    zlist_expand(&list, elemsize);

    size_t i;
    index *= elemsize;
    for(i = (zsize(list) + 1) * elemsize - 1; i > index; --i) {
        ((char*)list)[i] = ((char*)list)[i - elemsize];
    }

    ++*zsizeptr(list);

    return list;
}

void zlist_delete_(void *list, size_t elemsize, size_t index) {
    if(!list) { return; }
    
    size_t i;
    for(i = index * elemsize; i < (zsize(list) - 1) * elemsize; ++i) {
        ((char*)list)[i] = ((char*)list)[i + elemsize];
    }
    --*zsizeptr(list);
}

void zlist_backstep(void *list) {
    if(list) { --*zsizeptr(list); }
}

void zlist_rewind(void *list, size_t amount) {
    if(list) { *zsizeptr(list) -= amount; }
}

void zlist_clear(void *list) {
    if(list) { *zsizeptr(list) = 0; }
}
