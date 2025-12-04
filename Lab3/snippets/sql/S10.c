#include <sqlite3.h>
int delete_by_id(sqlite3 *db, int id) {
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "DELETE FROM items WHERE id = ?1;", -1, &s, NULL);
    sqlite3_bind_int(s,1,id);
    sqlite3_step(s);
    sqlite3_finalize(s);
    return 0;
}
