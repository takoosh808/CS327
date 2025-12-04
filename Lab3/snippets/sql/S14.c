#include <stdio.h>
#include <string.h>
int safe_email(const char *e) { return strlen(e) < 255 && strchr(e,'@'); }
