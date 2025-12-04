#include <stdio.h>
#include <string.h>
int main() {
    char a[10] = "hello";
    char b[10] = "world";
    char c[20];
    snprintf(c, sizeof c, "%s %s", a, b);
    puts(c);
}
