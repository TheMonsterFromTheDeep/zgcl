#include <stdlib.h>
#include <string.h>

#include "zstr.h"
#include "zdata.h"
#include "zexception.h"
#include "zbase.h"

static zstr *create(size_t size) {
    zstr *str = znew(size + 1, char);
    if(!str) {
        return NULL;
    }
    str[size] = '\0';
    --*zsizeptr(str);
    --*zallocptr(str);
    return str;
}

zstr *zstr_empty(void) {
    return create(0);
}

zstr *zstr_from(const char *base) {
    if(!base) { return zstr_empty(); }
    if(*base == '\0') { return zstr_empty(); }
    zstr *str = create(strlen(base));
    memcpy(str, base, zsize(str) + 1);
    return str;
}

zstr *zstr_copy(const char *base, size_t start, size_t end) {
    if(!base) { return zstr_empty(); }
    if(*base == '\0') { return zstr_empty(); }

    size_t i = 0;
    zstr *str;

    /* Special case: copy in reverse */
    if(start > end) {
        str = create(start - end);

        while(end > start) {
            str[i++] = base[end--];
        }

        return str;
    }

    str = create(end - start);

    while(start < end) {
        str[i] = base[start];
        if(base[start] == '\0') {
            *zsizeptr(str) = i + 1;
            break;
        }
        ++i;
        ++start;
    }

    return str;
}

zstr *zstr_clone(zstr *base) {
    if(!base) { return zstr_empty(); }
    
    zstr *str = create(zsize(base));
    memcpy(str, base, zsize(str) + 1);
    return str;
}

int zstr_eqz(const zstr *a, const zstr *b) {
    if(!a || !b) { return a == b; }
    if(zsize(a) != zsize(b)) { return FALSE; }
    return !strcmp(a, b);
}

int zstr_eqs(const zstr *a, const char *b) {
    if(!a || !b) { return (void*)a == (void*)b; }
    return !strcmp(a, b);
}

int zstr_eqc(const zstr *a, char b) {
    if(!a) { return FALSE; }
    if(zsize(a) != 1) { return FALSE; }
    return *a == b;
}

static void zstr_expand(zstr **a, size_t amount) {
    if(!a) { return; }
    if(!*a) { return; }
    size_t total = zsize(*a) + amount;
    if(total > zalloc(a)) {
        /* Double allocated amount unless that's not enough to cover 'amount'; in that case,
           allocate enough for 'amount'. */
        zstr *tmp = zrealloc(*a, 
            1 + (amount > zalloc(*a)) ? amount : zalloc(*a));
        if(!tmp) {
            throw("Error while reallocating string memory!");
            return;
        }
        *a = tmp;
        --*zallocptr(*a);
    }
    (*a)[total] = '\0';
}

void zstr_catz(zstr *str, const zstr *app) {
    if(!str || !app) { return; }

    size_t i;
    zstr_expand(&str, zsize(app));
    for(i = 0; i < zsize(app); ++i) {
        str[zsize(str) + i] = app[i];
    }
    *zsizeptr(str) += zsize(app);
}

void zstr_cats(zstr *str, const char *app) {
    if(!str || !app) { return; }

    size_t i, len = strlen(app);
    zstr_expand(&str, len);
    for(i = 0; i < len; ++i) {
        str[zsize(str) + i] = app[i];
    }
    *zsizeptr(str) += len;
}

void zstr_catc(zstr *str, char c) {
    if(!str) { return; }

    zstr_expand(&str, 1);
    str[(*zsizeptr(str))++] = c;
}

void zstr_insertz(zstr *str, size_t index, const zstr *ins) {
    if(!str || !ins) { return; }
    if(index > zsize(str)) { return; }
    
    size_t i;

    zstr_expand(&str, zsize(ins));

    for(i = zsize(str) + zsize(ins) + 1; i > index; --i) {
        str[i] = str[i - zsize(ins)]; 
    }

    for(i = 0; i < zsize(ins); ++i) {
        str[i + index] = ins[i];
    }
    *zsizeptr(str) += zsize(ins);
}

void zstr_inserts(zstr *str, size_t index, const char *ins) {
    if(!str || !ins) { return; }
    if(index > zsize(str)) { return; }
    
    size_t i, len = strlen(ins);
    zstr_expand(&str, len);
    for(i = zsize(str) + len + 1; i > index; --i) {
        str[i] = str[i - len]; 
    }

    for(i = 0; i < len; ++i) {
        str[i + index] = ins[i];
    }
    *zsizeptr(str) += len;
}

void zstr_insertc(zstr *str, size_t index, char ins) {
    if(!str) { return; }
    if(index > zsize(str)) { return; }
    
    size_t i;
    zstr_expand(&str, 1);
    for(i = zsize(str) + 2; i > index; --i) {
        str[i] = str[i - 1]; 
    }
    str[index] = ins;
    ++*zsizeptr(str);
}

void zstr_backspace(zstr *str) {
    if(!str) { return; }
    if(zsize(str) == 0) { return; }
    str[--*zsizeptr(str)] = '\0';
}

void zstr_rewind(zstr *str, size_t amount) {
    if(!str) { return; }
    if(zsize(str) == 0) { return; }
    *zsizeptr(str) -= amount;
    if(zsize(str) < 0) { *zsizeptr(str) = 0; }
    str[zsize(str)] = '\0';
}

void zstr_clear(zstr *str) {
    if(!str) { return; }
    *zsizeptr(str) = 0;
    *str = '\0';
}

void zstr_delete(zstr *str, size_t index) {
    if(!str) { return; }
    if(index > zsize(str)) { return; }
    
    size_t i;
    for(i = index; i < zsize(str); ++i) {
        str[i] = str[i + 1]; /* This will also copy null terminator */
    }
    --*zsizeptr(str);
}
