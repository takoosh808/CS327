#include <stdio.h>
int parse_and_use_id(const char *s) {
    char *e; long v = strtol(s,&e,10);
    if (*e) return -1; /* invalid */
    printf("safe id=%ld\n", v);
    return 0;
}
