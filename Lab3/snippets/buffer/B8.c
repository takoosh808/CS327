#include <stdio.h>
#include <ctype.h>
int sanitize(char *s, size_t n) {
    for (size_t i=0;i<n && s[i];++i) if (!isprint((unsigned char)s[i])) s[i]='?';
    return 0;
}
