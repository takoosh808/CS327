#include <stdio.h>
#include <string.h>
void safe_concat(char *dst, size_t dst_size, const char *src) {
    strncat(dst, src, dst_size - strlen(dst) - 1);
}
int main() {
    char s[32] = "prefix:";
    safe_concat(s, sizeof s, "more");
    puts(s);
}
