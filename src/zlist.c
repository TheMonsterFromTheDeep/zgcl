#include <stdlib.h>

#include "zexception.h"
#include "zlist.h"
#include "zbase.h"

size_t zlist_size(void *list) {
    if(!list) {
        throw("Cannot read size of null list!");
        return 0;
    }
    return ((size_t*)list)[-2];
}

size_t zlist_alloc(void *list) {
    if(!list) {
        throw("Cannot read allocated of null list!");
        return 0;
    }
    return ((size_t*)list)[-1];
}

void *zlist_new(size_t size, size_t elemsize) {
    void *tmp = malloc((sizeof(size_t) * 2) + elemsize * size);
    if(!tmp) {
        throw("Error while allocating list memory.");
        return NULL;
    }
    ((size_t*)tmp)[0] = size;
    ((size_t*)tmp)[1] = size;
    return ((size_t*)tmp) + 2;
}

size_t *zlist_size_ptr(void *list) {
    return ((size_t*)list) - 2;
}

size_t *zlist_alloc_ptr(void *list) {
    return ((size_t*)list) - 1;
}

size_t zlist_index(void *list, size_t index) {
    if(!list) {
        throw("Cannot read or write null list!");
        return 0;
    }
    if(index >= zlist_size(list)) {
        throw("Index out of list bounds!");
        return 0;
    }
    return index;
}

void *zlist_fit(void *list, size_t elemsize) {
    if(!list) {
        throw("Cannot expand null list!");
        return NULL;
    }
    if(zlist_size(list) >= zlist_alloc(list)) {
        size_t nsize = zlist_size(list) + 1;
        size_t nalloc = (zlist_alloc(list) * 2) + 1;
        void *tmp = realloc(list - 2 * sizeof(size_t), (nalloc * elemsize) + 2 * sizeof(size_t));
        if(!tmp) {
            throw("Error while reallocating list.");
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

void *zlist_expand(void *list, size_t elemsize, size_t newsize) {
    if(!list) {
        throw("Cannot expand null list!");
        return NULL;
    }
    if(zlist_size(list) >= zlist_alloc(list)) {
        size_t *alloc_ptr = zlist_alloc_ptr(list);
        *alloc_ptr += (*alloc_ptr > newsize) ? *alloc_ptr : newsize;
        void *tmp = realloc(list - 2 * sizeof(size_t), (*alloc_ptr * elemsize) + 2 * sizeof(size_t));
        if(!tmp) {
            throw("Error while reallocating list.");
            return list;
        }
        list = tmp + 2 * sizeof(size_t);
    }
    return list;
}

void zlist_free(void *list) {
    if(!list) { return; }
    free(((size_t*)list) - 2);
}
