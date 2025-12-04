#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
int safe_read(FILE *f, char *buf, size_t n) {
    if (!fgets(buf, n, f)) {
        if (feof(f)) return 0;
        return errno;
    }
    return 0;
}
