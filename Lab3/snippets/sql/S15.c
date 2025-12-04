#include <sqlite3.h>
void list_users(sqlite3 *db) {
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "SELECT id, name FROM users", -1, &s, NULL);
    while (sqlite3_step(s) == SQLITE_ROW) {
        printf("%d: %s\n", sqlite3_column_int(s,0), sqlite3_column_text(s,1));
    }
    sqlite3_finalize(s);
}
