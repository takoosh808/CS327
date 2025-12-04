#include <stdio.h>
#include <inttypes.h>
int main(void) {
    char fmtbuf[32];
    snprintf(fmtbuf, sizeof fmtbuf, "ID=%" " PRIu32, (uint32_t)1234);
    puts(fmtbuf);
}
