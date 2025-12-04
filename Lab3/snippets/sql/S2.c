#include <sqlite3.h>
int add_user(sqlite3 *db, const char *name, const char *email) {
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "INSERT INTO users(name,email) VALUES(?1,?2);", -1, &s, NULL);
    sqlite3_bind_text(s,1,name,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(s,2,email,-1,SQLITE_TRANSIENT);
    sqlite3_step(s);
    sqlite3_finalize(s);
    return 0;
}
