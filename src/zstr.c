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
    *zsizeptr(str) = size;
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
    size_t total = zsize(*a) + amount;
    if(total > zalloc(*a)) {
        /* Double allocated amount unless that's not enough to cover 'amount'; in that case,
           allocate enough for 'amount'. */
        zstr *tmp = zrealloc(*a, 
            zalloc(*a) + 1 + ((amount > zalloc(*a)) ? amount : zalloc(*a)));
        if(!tmp) {
            throw("Error while reallocating string memory!");
            return;
        }
        *a = tmp;
        --*zallocptr(*a);
    }
    (*a)[total] = '\0';
}

#define STR (*strptr)

void zstr_catz(zstr **strptr, const zstr *app) {
    if(!strptr || !app) { return; }
    if(!STR) { return; }

    size_t i;
    zstr_expand(strptr, zsize(app));
    for(i = 0; i < zsize(app); ++i) {
        STR[zsize(STR) + i] = app[i];
    }
    (*zsizeptr(STR)) += zsize(app);
}

void zstr_cats(zstr **strptr, const char *app) {
    if(!strptr || !app) { return; }
    if(!STR) { return; }
 
    size_t i, len = strlen(app);

    zstr_expand(strptr, len);

    for(i = 0; i < len; ++i) {
        STR[zsize(STR) + i] = app[i];
    }
    *zsizeptr(STR) += len;
}

void zstr_catc(zstr **strptr, char c) {
    if(!strptr) { return; }
    if(!STR) { return; }

    zstr_expand(strptr, 1);
    STR[zsize(STR)] = c;

    (*zsizeptr(STR)) += 1;
}

void zstr_insertz(zstr **strptr, size_t index, const zstr *ins) {
    if(!strptr || !ins) { return; }
    if(!STR) { return; }
    if(index > zsize(STR)) { return; }
    
    size_t i;

    zstr_expand(strptr, zsize(ins));

    for(i = zsize(STR) + zsize(ins) + 1; i > index; --i) {
        STR[i] = STR[i - zsize(ins)]; 
    }

    for(i = 0; i < zsize(ins); ++i) {
        STR[i + index] = ins[i];
    }
    *zsizeptr(STR) += zsize(ins);
}

void zstr_inserts(zstr **strptr, size_t index, const char *ins) {
    if(!strptr || !ins) { return; }
    if(!STR) { return; }
    if(index > zsize(STR)) { return; }
    
    size_t i, len = strlen(ins);
    zstr_expand(strptr, len);
    for(i = zsize(STR) + len + 1; i > index; --i) {
        STR[i] = STR[i - len]; 
    }

    for(i = 0; i < len; ++i) {
        STR[i + index] = ins[i];
    }
    *zsizeptr(STR) += len;
}

void zstr_insertc(zstr **strptr, size_t index, char ins) {
    if(!strptr) { return; }
    if(!STR) { return; }
    if(index > zsize(STR)) { return; }

    size_t i;
    zstr_expand(strptr, 1);
    for(i = zsize(STR) + 2; i > index; --i) {
        STR[i] = STR[i - 1]; 
    }
    STR[index] = ins;
    ++*zsizeptr(STR);
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
