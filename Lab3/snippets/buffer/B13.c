#include <stdio.h>
#include <string.h>
int main(void) {
    char buf[128];
    if (fgets(buf, sizeof buf, stdin)) {
        if (strlen(buf) < sizeof buf - 1) puts("safe");
    }
    return 0;
}
