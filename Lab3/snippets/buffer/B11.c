#include <stdio.h>
#include <string.h>
void safe_copy_or_trunc(char *dst, size_t n, const char *src) {
    strncpy(dst, src, n-1);
    dst[n-1] = '\0';
}
