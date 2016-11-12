#include <stdlib.h>

#include "zexception.h"
#include "zlist.h"
#include "zbase.h"

size_t zlist_size(void *list) {
    if(!list) {
        THROW("Cannot read size of null list!");
        return 0;
    }
    return ((size_t*)list)[-2];
}

size_t zlist_alloc(void *list) {
    if(!list) {
        THROW("Cannot read allocated of null list!");
        return 0;
    }
    return ((size_t*)list)[-1];
}

void *zlist_new(size_t size, size_t elemsize) {
    void *tmp = malloc((sizeof(size_t) * 2) + elemsize * size);
    if(!tmp) {
        THROW("Error while allocating list memory.");
        return NULL;
    }
    ((size_t*)tmp)[0] = size;
    ((size_t*)tmp)[1] = size;
    return ((size_t*)tmp) + 2;
}

static size_t *zlist_size_ptr(void *list) {
    return ((size_t*)list) - 2;
}

static size_t *zlist_alloc_ptr(void *list) {
    return ((size_t*)list) - 1;
}

int zlist_validate(void *list, size_t index) {
    if(!list) {
        THROW("Cannot read from null list!");
        return FALSE;
    }
    if(index >= zlist_size(list)) {
        THROW("Index out of list bounds!");
        return FALSE;
    }
    return TRUE;
}

void *zlist_fit(void *list, size_t elemsize) {
    if(!list) {
        THROW("Cannot expand null list!");
        return NULL;
    }
    if(zlist_size(list) >= zlist_alloc(list)) {
        size_t nsize = zlist_size(list) + 1;
        size_t nalloc = (zlist_alloc(list) * 2) + 1;
        void *tmp = realloc(list - 2 * sizeof(size_t), (nalloc * elemsize) + 2 * sizeof(size_t));
        if(!tmp) {
            THROW("Error while reallocating list.");
            return list;
        }
        list = tmp + 2 * sizeof(size_t);
        ((size_t*)tmp)[0] = nsize;
        ((size_t*)tmp)[1] = nalloc;
    }
    else {
        (*zlist_size_ptr(list))++;
    }
    return list;
}

void zlist_free(void *list) {
    if(!list) { return; }
    free(zlist_size_ptr(list));
}
