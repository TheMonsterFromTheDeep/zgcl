#ifndef ZSTR_H
#define ZSTR_H

#include <stdlib.h>

typedef struct zstr {
    char *data;
    size_t size;
    size_t alloc;
} zstr;

zstr *zstr_empty(void);
zstr *zstr_from(const char*);

int zstr_eq(zstr*,zstr*);
int zstr_eqc(zstr*,const char*);

void zstr_expand(zstr*,size_t);

void zstr_cat(zstr*,const zstr*);
void zstr_catc(zstr*,const char*);

void zstr_free(zstr*);

#endif
