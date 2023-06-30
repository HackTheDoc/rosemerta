#include "include/Database.h"

#include <iostream>

std::string Database::path = "null";
sqlite3* Database::db = nullptr;

void Database::SetPath(std::string path) {
    Database::path = path;
}

void Database::Create() {
    sqlite3_open(path.c_str(), &db);

    std::string identityQuery = "CREATE TABLE IF NOT EXISTS \"identity\" (\"id\" INTEGER UNIQUE, \"username\" TEXT,	\"name\" TEXT, \"lastname\"	TEXT, \"age\" INTEGER, \"birthday\"	TEXT, \"status\" INTEGER, \"notes\"	TEXT, PRIMARY KEY(\"id\"));";
    if (sqlite3_exec(db, identityQuery.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string contactQuery = "CREATE TABLE IF NOT EXISTS \"contact\" (\"owner\" INTEGER, \"type\" INTEGER, \"detail\" TEXT);";
    if (sqlite3_exec(db, contactQuery.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_close(db);
}
