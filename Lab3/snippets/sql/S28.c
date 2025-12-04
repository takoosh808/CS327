#include <sqlite3.h>
int select_limit(sqlite3 *db, int limit) {
    if (limit < 1 || limit > 1000) return -1;
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "SELECT * FROM logs LIMIT ?1;", -1, &s, NULL);
    sqlite3_bind_int(s,1,limit);
    sqlite3_finalize(s);
    return 0;
}
