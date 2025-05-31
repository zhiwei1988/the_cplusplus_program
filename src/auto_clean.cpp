#include <stdio.h>
#include <stdlib.h>

void cleanup_int(void *ptr) {
    if (ptr) {
        int32_t *p = *(int32_t **)ptr;
        printf("Cleaning up %p\n", p);
        free(p);
    }
}
