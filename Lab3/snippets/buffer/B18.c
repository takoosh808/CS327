#include <stdio.h>
#include <string.h>
void safe_insert(char *buf, size_t bufsize, const char *ins) {
    size_t cur = strlen(buf);
    strncat(buf, ins, bufsize - cur - 1);
}
