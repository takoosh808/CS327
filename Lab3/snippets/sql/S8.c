#include <sqlite3.h>
int count_users(sqlite3 *db, const char *email) {
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM users WHERE email = ?1;", -1, &s, NULL);
    sqlite3_bind_text(s,1,email,-1,SQLITE_STATIC);
    if (sqlite3_step(s) == SQLITE_ROW) printf("count=%d\n", sqlite3_column_int(s,0));
    sqlite3_finalize(s);
    return 0;
}
