#ifndef ZEXCEPTION_H
#define ZEXCEPTION_H

#include <setjmp.h>
#include <stdio.h>

#include "zstr.h"

jmp_buf except_buf;
extern void throw(const char *msg);
extern const char *error_msg(void);

#define TRY if(!setjmp(except_buf))
#define CATCH else

#ifdef Z_ENTRY
typedef struct zargs {
    int count;
    char** get;
} zargs;

extern void entry(zargs);
extern void set_exit_handler(int (*)(void));
#endif

extern int main(int,char*[]); 

#endif
