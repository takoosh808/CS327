#include <stdio.h>
#include <string.h>
int safe_substr(char *out, size_t outn, const char *s, size_t off, size_t len) {
    if (off >= strlen(s)) { out[0] = '\0'; return -1; }
    size_t actual = strlen(s) - off;
    if (len > actual) len = actual;
    strncpy(out, s + off, (len < outn-1)?len:outn-1);
    out[(len < outn-1)?len:outn-1] = '\0';
    return 0;
}
