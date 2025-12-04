#include <stdio.h>
#include <limits.h>
int parse_int(const char *s) {
    long v = strtol(s, NULL, 10);
    if (v < INT_MIN || v > INT_MAX) return 0;
    return (int)v;
}
