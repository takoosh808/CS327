#include <sqlite3.h>
#include <stdio.h>
void use_mprintf(sqlite3 *db, const char *name) {
    char *q = sqlite3_mprintf("SELECT * FROM users WHERE name=%Q;", name);
    sqlite3_free(q);
}
