#include "auto_clean.h"
#include <stdint.h>
#include <stdlib.h>


static bool g_is_cleanup = false;

int main() {
    if (!g_is_cleanup) {
        return 0;
    }
    int32_t *ptr = (int32_t *)malloc(sizeof(int32_t) * 10);
    MEDIA_AUTO_CLEAN_RESOURCE1(cleanup_int, ptr);
    return 0;
}