#include <stdio.h>
#include <string.h>
int concat_three(char *dst, size_t n, const char *a, const char *b, const char *c) {
    if (strlen(a)+strlen(b)+strlen(c)+1 > n) return -1;
    strcpy(dst, a);
    strcat(dst, b);
    strcat(dst, c);
    return 0;
}
