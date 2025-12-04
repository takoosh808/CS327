#include <sqlite3.h>
int search_like(sqlite3 *db, const char *term) {
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "SELECT name FROM users WHERE name LIKE ?1;", -1, &s, NULL);
    char pattern[128];
    snprintf(pattern, sizeof pattern, "%%%s%%", term);
    sqlite3_bind_text(s,1,pattern,-1,SQLITE_TRANSIENT);
    sqlite3_finalize(s);
    return 0;
}
