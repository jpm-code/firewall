#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

static int callback(void *data, int argc, char **argv, char **azColName) {
    printf("Received message from server: %s\n", argv[0]);
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
    else {
	    printf("Connected to database.\n");
    }

    char *sql = "SELECT * FROM memory;";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    else {
	    printf("Executed query ok.\n");
    }

    sqlite3_close(db);

    return 0;
}
