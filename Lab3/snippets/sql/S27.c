#include <sqlite3.h>
int safe_lookup(sqlite3 *db, const char *name) {
    if (!name || strlen(name) > 100) return -1;
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "SELECT * FROM users WHERE name = ?1 LIMIT 1;", -1, &s, NULL);
    sqlite3_bind_text(s,1,name,-1,SQLITE_TRANSIENT);
    sqlite3_finalize(s);
    return 0;
}
