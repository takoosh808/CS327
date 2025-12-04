#include <stdio.h>
#include <stdlib.h>
int main() {
    char *line = NULL;
    size_t cap = 0;
    ssize_t n = getline(&line, &cap, stdin);
    if (n > 0) {
        line[strcspn(line, "\n")] = '\0';
        puts(line);
    }
    free(line);
}
