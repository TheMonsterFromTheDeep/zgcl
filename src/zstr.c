#include <stdlib.h>
#include <string.h>

#include "zstr.h"
#include "zexception.h"
#include "zbase.h"

static zstr *create(size_t size) {
    zstr *str = malloc(sizeof(zstr));
    char *tmp = malloc(1 + sizeof(char) * size);
    if(!tmp) {
        throw("Error while allocating string memory!");
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
    if(!base) { return zstr_empty(); }
    if(*base == '\0') { return zstr_empty(); }
    zstr *str = create(strlen(base));
    memcpy(str->data, base, str->size + 1);
    return str;
}

zstr *zstr_copy(const char *base, size_t start, size_t end) {
    if(!base) { return zstr_empty(); }
    if(*base == '\0') { return zstr_empty(); }
    zstr *str = create(end - start);

    size_t i = 0;

    while(start < end) {
        str->data[i] = base[start];
        if(base[start] == '\0') {
            str->size = i + 1;
        }
        ++i;
        ++start;
    }

    return str;
}

char zstr_at(zstr *str, size_t index) {
    if(!str) {
        throw("Cannot read from null string!");
        return 0;
    }
    if(index > str->size) {
        throw("Index out of string bounds!");
        return 0;
    }
    return str->data[index];
}

int zstr_eqz(const zstr *a, const zstr *b) {
    if(!a || !b) { return a == b; }
    if(!a->data || !b->data) { return a->data == b->data; }
    if(a->size != b->size) { return FALSE; }
    return !strcmp(a->data, b->data);
}

int zstr_eqs(const zstr *a, const char *b) {
    if(!a || !b) { return (void*)a == (void*)b; }
    if(!a->data) { return FALSE; }
    return !strcmp(a->data, b);
}

int zstr_eqc(const zstr *a, char b) {
    if(!a) { return FALSE; }
    if(a->size != 1) { return FALSE; }
    return *a->data == b;
}

void zstr_expand(zstr *a, size_t amount) {
    if(!a) { return; }
    if(!a->data) { return; }
    size_t total = a->size + amount;
    if(total > a->alloc) {
        /* Double allocated amount unless that's not enough to cover 'amount'; in that case,
           allocate enough for 'amount'. */
        a->alloc += (amount > a->alloc) ? amount : a->alloc;
        char *tmp = realloc(a->data, sizeof(char) * (a->alloc + 1));
        if(!tmp) {
            throw("Error while reallocating string memory!");
            return;
        }
        a->data = tmp;
    }
    a->data[total] = '\0';
}

void zstr_catz(zstr *str, const zstr *app) {
    if(!str || !app) { return; }
    if(!str->data || !app->data) { return; }

    size_t i;
    zstr_expand(str, app->size);
    for(i = 0; i < app->size; ++i) {
        str->data[str->size + i] = app->data[i];
    }
    str->size += app->size;
}

void zstr_cats(zstr *str, const char *app) {
    if(!str || !app) { return; }
    if(!str->data) { return; }

    size_t i, len = strlen(app);
    zstr_expand(str, len);
    for(i = 0; i < len; ++i) {
        str->data[str->size + i] = app[i];
    }
    str->size += len;
}

void zstr_catc(zstr *str, char c) {
    if(!str) { return; }
    if(!str->data) { return; }

    zstr_expand(str, 1);
    str->data[str->size++] = c;
}

void zstr_insertz(zstr *str, size_t index, const zstr *ins) {
    if(!str || !ins) { return; }
    if(!str->data || !ins->data) { return; }
    if(index > str->size) {
        throw("Insertion out of string bounds!");
        return;
    }
    
    size_t i;
    zstr_expand(str, ins->size);
    for(i = str->size + ins->size + 1; i > index; --i) {
        str->data[i] = str->data[i - ins->size]; 
    }

    for(i = 0; i < ins->size; ++i) {
        str->data[i + index] = ins->data[i];
    }
    str->size += ins->size;
}

void zstr_inserts(zstr *str, size_t index, const char *ins) {
    if(!str || !ins) { return; }
    if(!str->data) { return; }
    if(index > str->size) {
        throw("Insertion out of string bounds!");
        return;
    }
    
    size_t i, len = strlen(ins);
    zstr_expand(str, len);
    for(i = str->size + len + 1; i > index; --i) {
        str->data[i] = str->data[i - len]; 
    }

    for(i = 0; i < len; ++i) {
        str->data[i + index] = ins[i];
    }
    str->size += len;
}

void zstr_insertc(zstr *str, size_t index, char ins) {
    if(!str) { return; }
    if(!str->data) { return; }
    if(index > str->size) {
        throw("Insertion out of string bounds!");
        return;
    }
    
    size_t i;
    zstr_expand(str, 1);
    for(i = str->size + 2; i > index; --i) {
        str->data[i] = str->data[i - 1]; 
    }
    str->data[index] = ins;
    ++str->size;
}

void zstr_backspace(zstr *str) {
    if(!str) { return; }
    if(!str->data) { return; }
    if(str->size == 0) { return; }
    str->data[--str->size] = '\0';
}

void zstr_clear(zstr *str) {
    if(!str) { return; }
    if(!str->data) { return; }
    str->size = 0;
    str->data[0] = '\0';
}

void zstr_free(zstr *str) {
    if(!str) { return; }
    free(str->data);
    free(str);
}
