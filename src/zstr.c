#include <stdlib.h>
#include <string.h>

#include "zstr.h"
#include "zexception.h"
#include "zbase.h"

static zstr *create(size_t size) {
    zstr *str = malloc(sizeof(zstr));
    char *tmp = malloc(1 + sizeof(char) * size);
    if(!tmp) {
        THROW("Error while allocating string memory!");
        return NULL;
    }
    str->data = tmp;
    str->data[size] = '\0';
    str->size = size;
    str->alloc = size;
    return str;
}

zstr *zstr_empty(void) {
    return create(0);
}

zstr *zstr_from(const char *base) {
    if(base == NULL) { return zstr_empty(); }
    if(*base == '\0') { return zstr_empty(); }
    zstr *str = create(strlen(base));
    memcpy(str->data, base, str->size + 1);
    return str;
}

char zstr_at(zstr *str, size_t index) {
    if(str == NULL) {
        THROW("Cannot read from null string!");
        return 0;
    }
    if(index > str->size) {
        THROW("Index out of string bounds!");
        return 0;
    }
    return str->data[index];
}

int zstr_eq(zstr *a, zstr *b) {
    if(a == NULL || b == NULL) { return a == b; }
    if(a->size != b->size) { return FALSE; }
    return !strcmp(a->data, b->data);
}

int zstr_eqc(zstr *a, const char *b) {
    if(a == NULL || b == NULL) { return (void*)a == (void*)b; }
    return !strcmp(a->data, b);
}

void zstr_expand(zstr *a, size_t amount) {
    if(!a) { return; }
    size_t total = a->size + amount;
    if(total < a->alloc) {
        a->alloc += (amount > a->alloc) ? amount : a->alloc;
        char *tmp = realloc(a->data, sizeof(char) * a->alloc + 1);
        if(!tmp) {
            THROW("Error while reallocating string memory!");
            return;
        }
        a->data = tmp;
    }
    a->data[total] = '\0';
}

void zstr_cat(zstr *str, const zstr *app) {
    if(!str || !app) { return; }
    size_t i;
    zstr_expand(str, app->size);
    for(i = 0; i < app->size; ++i) {
        str->data[str->size + i] = app->data[i];
    }
    str->size += app->size;
}

void zstr_catc(zstr *str, const char *app) {
    if(!str || !app) { return; }
    size_t i, len = strlen(app);
    zstr_expand(str, len);
    for(i = 0; i < len; ++i) {
        str->data[str->size + i] = app[i];
    }
    str->size += len;
}

void zstr_catb(zstr *str, char c) {
    if(!str) { return; }
    zstr_expand(str, 1);
    str->data[str->size++] = c;
}

void zstr_backspace(zstr *str) {
    if(!str) { return; }
    if(!str->data) { return; }
    if(str->size == 0) { return; }
    str->data[--str->size] = '\0';
}

void zstr_free(zstr *str) {
    if(str == NULL) { return; }
    free(str->data);
    free(str);
}
