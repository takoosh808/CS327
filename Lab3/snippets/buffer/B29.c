#include <stdio.h>
#include <stdlib.h>
int main() {
    char name[32];
    if (scanf("%31s", name)==1) printf("Name ok\n");
    return 0;
}
