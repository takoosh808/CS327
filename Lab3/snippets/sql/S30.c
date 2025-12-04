#include <stdio.h>
#include <string.h>
int safe_use_schema(const char *schema) {
    const char *allowed[] = {"public","app","test"};
    for (size_t i=0;i<sizeof allowed/sizeof *allowed;++i) if (strcmp(schema,allowed[i])==0) return 0;
    return -1;
}
