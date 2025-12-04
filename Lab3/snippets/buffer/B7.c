#include <stdio.h>
#include <stdint.h>
int main() {
    char buf[20];
    size_t read = fread(buf, 1, sizeof buf - 1, stdin);
    buf[read] = '\0';
    puts(buf);
    return 0;
}
