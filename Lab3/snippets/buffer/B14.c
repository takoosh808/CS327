#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *dup_safe(const char *s) {
    size_t len = strlen(s) + 1;
    char *d = malloc(len);
    if (!d) return NULL;
    memcpy(d, s, len);
    return d;
}
