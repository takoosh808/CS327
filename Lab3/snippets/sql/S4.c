#include <sqlite3.h>
#include <stdio.h>
int main(void) {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    sqlite3_exec(db, "CREATE TABLE t(id INT, name TEXT);",0,0,0);
    sqlite3_close(db);
    return 0;
}
