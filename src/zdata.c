#include <stdlib.h>

#define HEAD(ptr) (ptr - (2 * sizeof(size_t)))
#define START(ptr) (ptr + (2 * sizeof(size_t)))
#define SIZE(s) ((sizeof(size_t) * 2) + (s))

size_t zsize(const void *struc) {
    if(!struc) { return 0; }
    return ((size_t*)struc)[-2];
}

size_t zalloc(const void *struc) {
    if(!struc) { return 0; }
    return ((size_t*)struc)[-1];
}

size_t *zsizeptr(void *struc) {
    return ((size_t*)struc) - 2;
}

size_t *zallocptr(void *struc) {
    return ((size_t*)struc) - 1;
}

void *znewt(size_t size, size_t elemsize) {
    void *tmp = malloc(SIZE(elemsize * size));
    if(!tmp) {
        return NULL;
    }
    ((size_t*)tmp)[0] = size;
    ((size_t*)tmp)[1] = size;
    return ((size_t*)tmp) + 2;
}

void *zrealloct(void *struc, size_t alloc, size_t elemsize) {
    size_t size = zsize(struc);
    if(alloc < size) { size = alloc; }
    void *tmp = realloc(HEAD(struc), SIZE(elemsize * alloc));
    if(!tmp) {
        return NULL; /* Failure */
    }
    ((size_t*)tmp)[0] = size;
    ((size_t*)tmp)[1] = alloc;
    return START(tmp);
}

void zfree(void *struc) {
    if(struc) { free(HEAD(struc)); }
}
