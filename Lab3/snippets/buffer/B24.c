#include <stdio.h>
#include <string.h>
void format_user(char *dst, size_t n, const char *user) {
    snprintf(dst, n, "user=%.30s", user);
}
