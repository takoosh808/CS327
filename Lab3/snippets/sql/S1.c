#include <sqlite3.h>
#include <stdio.h>
int query_user(sqlite3 *db, const char *name) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, email FROM users WHERE name = ?1;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("id=%d email=%s\n", sqlite3_column_int(stmt,0), sqlite3_column_text(stmt,1));
    }
    sqlite3_finalize(stmt);
    return 0;
}
