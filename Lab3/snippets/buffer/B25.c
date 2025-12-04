#include <stdio.h>
#include <stdlib.h>
int main() {
    char *buf = calloc(1, 64);
    if (!buf) return 1;
    fgets(buf, 64, stdin);
    puts(buf);
    free(buf);
}
