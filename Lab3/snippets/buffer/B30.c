#include <stdio.h>
#include <string.h>
int compare_safe(const char *a, const char *b, size_t maxlen) {
    return strncmp(a, b, maxlen);
}
