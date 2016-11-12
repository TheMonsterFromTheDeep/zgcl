#include <stdio.h>
#include <setjmp.h>

#include "zentry.h"

#define EXIT if(exit_handler) { return exit_handler(); } else return 0

int main(int argc, char *argv[]) {
    exit_handler = NULL;

    if(setjmp(except_buf)) {
        printf("Exiting: %s\n", except_msg);
        EXIT;
    }

    entry((zargs){ argc, argv });
    EXIT;
}
