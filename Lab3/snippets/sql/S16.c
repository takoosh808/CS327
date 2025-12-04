#include <sqlite3.h>
int update_email(sqlite3 *db, int id, const char *email) {
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "UPDATE users SET email=?1 WHERE id=?2;", -1, &s, NULL);
    sqlite3_bind_text(s,1,email,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int(s,2,id);
    sqlite3_step(s);
    sqlite3_finalize(s);
    return 0;
}
