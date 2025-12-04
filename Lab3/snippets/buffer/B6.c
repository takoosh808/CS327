#include <stdio.h>
#include <string.h>
int copy_safe(char *dst, size_t n, const char *src) {
    if (strlen(src) >= n) return -1;
    strcpy(dst, src);
    return 0;
}
