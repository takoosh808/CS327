#include <stdio.h>
#include <string.h>
int safe_build(char *out, size_t n, const char *name) {
    if (strlen(name) > 50) return -1;
    snprintf(out, n, "SELECT * FROM users WHERE name='%s';", name);
    return 0;
}
