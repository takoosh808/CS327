#include <sqlite3.h>
void explicit_columns(sqlite3 *db) {
    sqlite3_prepare_v2(db, "SELECT id, name FROM users WHERE active=1;", -1, NULL, NULL);
}
