#include <sqlite3.h>
int insert_blob(sqlite3 *db, const void *data, int len) {
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "INSERT INTO bin(d) VALUES(?1);", -1, &s, NULL);
    sqlite3_bind_blob(s,1,data,len,SQLITE_TRANSIENT);
    sqlite3_step(s);
    sqlite3_finalize(s);
    return 0;
}
