#include <sqlite3.h>
int bind_if_not_null(sqlite3_stmt *s, int idx, const char *val) {
    if (val) sqlite3_bind_text(s, idx, val, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(s, idx);
    return 0;
}
