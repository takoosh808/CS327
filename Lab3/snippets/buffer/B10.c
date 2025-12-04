#include <stdio.h>
#include <stdlib.h>
int read_line(char *buf, size_t cap) {
    if (!fgets(buf, cap, stdin)) return -1;
    buf[strcspn(buf, "\n")] = '\0';
    return 0;
}
