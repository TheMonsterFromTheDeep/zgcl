#include <stdio.h>
#include <setjmp.h>

#include "zentry.h"

static int (*exit_handler)(void);
static const char *except_msg;

void throw(const char *msg) {
    except_msg = msg;
    longjmp(except_buf, 0);
}

const char *error_msg() {
    return except_msg;
}

int main(int argc, char *argv[]) {
    exit_handler = NULL;

    if(setjmp(except_buf)) {
        printf("Exiting: %s\n", except_msg);
        return exit_handler ? exit_handler() : 0;
    }

    entry((zargs){ argc, argv });
    return exit_handler ? exit_handler() : 0;
}
