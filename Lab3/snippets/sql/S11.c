#include <sqlite3.h>
int add_two_safe(sqlite3 *db, const char *a, const char *b) {
    sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "INSERT INTO x(val) VALUES(?1);", -1, &s, NULL);
    sqlite3_bind_text(s,1,a,-1,SQLITE_TRANSIENT);
    sqlite3_step(s); sqlite3_reset(s);
    sqlite3_bind_text(s,1,b,-1,SQLITE_TRANSIENT);
    sqlite3_step(s); sqlite3_finalize(s);
    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    return 0;
}
