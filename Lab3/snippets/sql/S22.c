#include <sqlite3.h>
int safe_prepare(sqlite3 *db, const char *sql) {
    sqlite3_stmt *s;
    int rc = sqlite3_prepare_v2(db, sql, -1, &s, NULL);
    if (rc != SQLITE_OK) return rc;
    sqlite3_finalize(s);
    return SQLITE_OK;
}
