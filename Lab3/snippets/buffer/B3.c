#include <stdio.h>
#include <stdlib.h>
int main() {
    size_t len = 128;
    char *buf = malloc(len);
    if (!buf) return 1;
    if (fgets(buf, len, stdin)) puts(buf);
    free(buf);
    return 0;
}
