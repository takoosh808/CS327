#include <stdio.h>
#include <string.h>
void safe_format(char *out, size_t outn, const char *name) {
    snprintf(out, outn, "Name: %.20s", name);
}
