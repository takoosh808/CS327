#include <sqlite3.h>
int joined_query(sqlite3 *db, const char *role) {
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "SELECT u.id, r.name FROM users u JOIN roles r ON u.role=r.id WHERE r.name=?1;", -1, &s, NULL);
    sqlite3_bind_text(s,1,role,-1,SQLITE_TRANSIENT);
    sqlite3_finalize(s);
    return 0;
}
