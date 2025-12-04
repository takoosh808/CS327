#include <sqlite3.h>
int fts_search(sqlite3 *db, const char *term) {
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "SELECT rowid, snippet(fts) FROM fts WHERE fts MATCH ?1;", -1, &s, NULL);
    sqlite3_bind_text(s,1,term,-1,SQLITE_TRANSIENT);
    sqlite3_finalize(s);
    return 0;
}
