#ifndef ZEXCEPTION_H
#define ZEXCEPTION_H

#include <setjmp.h>
#include <stdio.h>

#include "zstr.h"

jmp_buf except_buf;

const char *except_msg;

#define TRY if(!setjmp(except_buf))
#define CATCH else

#define THROW(msg) do { except_msg = (msg); longjmp(except_buf,0); } while(0)

#ifdef Z_ENTRY
typedef struct zargs {
    int count;
    char** get;
} zargs;

void entry(zargs);
int (*exit_handler)(void);

int main(int,char*[]); 
#endif

#endif
