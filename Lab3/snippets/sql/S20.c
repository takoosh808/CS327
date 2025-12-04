#include <stdio.h>
#include <string.h>
int build_and_prepare(char *q, size_t n, const char *tbl) {
    if (strlen(tbl) > 30) return -1;
    snprintf(q, n, "SELECT * FROM %s WHERE 1=1;", tbl);
    return 0;
}
