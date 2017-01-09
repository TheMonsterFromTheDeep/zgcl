#ifndef ZSTR_H
#define ZSTR_H

#include <stdlib.h>

typedef char zstr;

extern zstr *zstr_empty(void);
extern zstr *zstr_from(const char*);
extern zstr *zstr_copy(const char*,size_t,size_t);

extern int zstr_eqz(const zstr*,const zstr*);
extern int zstr_eqs(const zstr*,const char*);
extern int zstr_eqc(const zstr*,char);

extern void zstr_catz(zstr*,const zstr*);
extern void zstr_cats(zstr*,const char*);
extern void zstr_catc(zstr*,char);

extern void zstr_insertz(zstr*,size_t,const zstr*);
extern void zstr_inserts(zstr*,size_t,const char*);
extern void zstr_insertc(zstr*,size_t,char);

extern void zstr_backspace(zstr*);
extern void zstr_rewind(zstr*,size_t);
extern void zstr_clear(zstr*);

extern void zstr_delete(zstr*,size_t);

#endif /* Header guard */
