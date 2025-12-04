#include <stdio.h>
#include <stdlib.h>
int id_in_range(const char *s) {
    char *end;
    long v = strtol(s, &end, 10);
    return (*end=='\0' && v>=0 && v<=1000000);
}
