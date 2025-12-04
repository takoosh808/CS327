#include <stdio.h>
#include <string.h>
int main() {
    char name[50];
    if (fgets(name, sizeof name, stdin)) {
        name[strcspn(name, "\n")] = '\0';
        printf("Hello %s\n", name);
    }
    return 0;
}
