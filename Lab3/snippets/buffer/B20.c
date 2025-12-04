#include <stdio.h>
#include <string.h>
void zero_and_copy(char *dst, size_t n, const char *src) {
    memset(dst, 0, n);
    strncpy(dst, src, n-1);
}
