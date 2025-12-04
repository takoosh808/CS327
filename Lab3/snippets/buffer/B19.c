#include <stdio.h>
#include <stdlib.h>
int main() {
    char *p = malloc(32);
    if (!p) return 1;
    snprintf(p, 32, "%s", "constant");
    puts(p);
    free(p);
}
