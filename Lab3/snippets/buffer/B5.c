#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    char fixed[16];
    snprintf(fixed, sizeof fixed, "%d-%s", 42, "OK");
    puts(fixed);
    return 0;
}
