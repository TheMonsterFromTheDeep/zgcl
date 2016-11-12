#include <stdio.h>
#include <setjmp.h>

#include "zentry.h"

int main(int argc, char *argv[]) {
    if(setjmp(except_buf)) {
        printf("Exiting: %s\n", except_msg);
        return 0;
    }
    return entry((zargs){ argc, argv });
}
