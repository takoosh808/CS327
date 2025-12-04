#include <stdio.h>
#include <stdlib.h>
int safe_read_ints(int *arr, size_t n) {
    for (size_t i=0;i<n;++i) if (scanf("%d", &arr[i])!=1) return -1;
    return 0;
}
