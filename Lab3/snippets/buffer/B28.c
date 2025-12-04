#include <stdio.h>
#include <string.h>
void safe_replace(char *s, size_t n, char a, char b) {
    for (size_t i=0;i<n && s[i];++i) if (s[i]==a) s[i]=b;
}
