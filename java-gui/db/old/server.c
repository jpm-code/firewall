#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

static int callback(void *data, int argc, char **argv, char **azColName) {
    return 0;
}

int main() {
    sqlite3 *db;
    char *err_msg = 0;
    int rc;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open in-memory database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    char *sql = "CREATE TABLE memory (data TEXT);"
                "INSERT INTO memory VALUES('Hello from server!');";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    printf("Server is running. Press ENTER to exit.\n");
while(1==1) {}

    getchar();

    sqlite3_close(db);

    return 0;
}

