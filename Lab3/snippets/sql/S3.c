/* Pseudocode demonstrating prepared stmt pattern (MySQL-style) */
#include <mysql/mysql.h>
void safe_query(MYSQL *conn, const char *username) {
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    const char *q = "SELECT id FROM users WHERE username = ?";
    mysql_stmt_prepare(stmt, q, strlen(q));
    MYSQL_BIND bind;
    memset(&bind,0,sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.buffer = (char*)username;
    bind.buffer_length = strlen(username);
    mysql_stmt_bind_param(stmt, &bind);
    mysql_stmt_execute(stmt);
    mysql_stmt_close(stmt);
}
