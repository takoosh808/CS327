#include <stdio.h>
#include <string.h>
int valid_name(const char *s) {
    return strlen(s) < 100; /* explicit validation */
}
