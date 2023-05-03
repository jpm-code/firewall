// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <string>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lmdb.h"

void check(int rc, const char* msg) {
    if (rc != MDB_SUCCESS) {
        fprintf(stderr, "%s: %s\n", msg, mdb_strerror(rc));
        exit(rc);
    }
}


int main() {
    MDB_env* env;
    MDB_dbi dbi;
    MDB_txn* txn;
    MDB_cursor* cursor;

    // Create and open an LMDB environment
    std::string db_path = "./lmdb_database";
    check(mdb_env_create(&env), "Failed to create environment");
    check(mdb_env_set_mapsize(env, 10485760), "Failed to set map size"); // Set the map size to 10 MB
    check(mdb_env_open(env, db_path.c_str(), 0, 0664), "Failed to open environment");

    // Begin a new transaction and open a new database
    check(mdb_txn_begin(env, nullptr, 0, &txn), "Failed to begin transaction");
    check(mdb_dbi_open(txn, nullptr, 0, &dbi), "Failed to open database");

    // Store key-value pairs
    MDB_val key, value;
    for (int i = 1; i <= 5; ++i) {
        key.mv_size = sizeof(int);
        key.mv_data = &i;

        std::string value_str = "Value_" + std::to_string(i);
        value.mv_size = value_str.size();
        value.mv_data = const_cast<char*>(value_str.data());

        check(mdb_put(txn, dbi, &key, &value, 0), "Failed to put data");
    }

    //Use a text string for a key now
    std::string key_str = "domain.com";
    int value_int = 12345;

    key.mv_size = key_str.size();
    key.mv_data = const_cast<char*>(key_str.data());

    value.mv_size = sizeof(int);
    value.mv_data = &value_int;

    check(mdb_put(txn, dbi, &key, &value, 0), "Failed to put data");


    // Commit the transaction
    check(mdb_txn_commit(txn), "Failed to commit transaction");

    // Begin a new read-only transaction
    check(mdb_txn_begin(env, nullptr, MDB_RDONLY, &txn), "Failed to begin read transaction");

    // Open a cursor
    check(mdb_cursor_open(txn, dbi, &cursor), "Failed to open cursor");

    // Iterate through the key-value pairs and print them
    /*while (mdb_cursor_get(cursor, &key, &value, MDB_NEXT) == 0) {
        int key_int = *static_cast<int*>(key.mv_data);
        std::string value_str(static_cast<char*>(value.mv_data), value.mv_size);
        std::cout << "Key: " << key_int << ", Value: " << value_str << std::endl;
    }*/


    // Access randomly for int key
    int search_key = 3; // Key to search for
    key.mv_size = sizeof(int);
    key.mv_data = &search_key;

    int rc = mdb_get(txn, dbi, &key, &value);

    if (rc == MDB_SUCCESS) {
        int key_int = *static_cast<int*>(key.mv_data);
        std::string value_str(static_cast<char*>(value.mv_data), value.mv_size);
        std::cout << "Key: " << key_int << ", Value: " << value_str << std::endl;
    }
    else {
        std::cerr << "Failed to get value for key " << search_key << ": " << mdb_strerror(rc) << std::endl;
    }


   
    // Access randomly for string key
    std::string search_key_str = "domain.com"; // Key to search for
    key.mv_size = search_key_str.size();
    key.mv_data = const_cast<char*>(search_key_str.data());

    rc = mdb_get(txn, dbi, &key, &value);
    if (rc == MDB_SUCCESS) {
        std::string key_str(static_cast<char*>(key.mv_data), key.mv_size);
        int value_int = *static_cast<int*>(value.mv_data);
        std::cout << "Key: " << key_str << ", Value: " << value_int << std::endl;
    }
    else {
        std::cerr << "Failed to get value for key " << search_key_str << ": " << mdb_strerror(rc) << std::endl;
    }

    // Close the cursor, abort the transaction, close the database, and close the environment
    mdb_cursor_close(cursor);
    mdb_txn_abort(txn);
    mdb_dbi_close(env, dbi);
    mdb_env_close(env);

    return 0;
}

