#include "include/Database.h"

#include <iostream>

std::string Database::path = "null";
sqlite3* Database::db = nullptr;

void Database::SetPath(std::string path) {
    Database::path = path;
}

void Database::Create() {
    sqlite3_open(path.c_str(), &db);

    const char* identitiesTable =
        "CREATE TABLE \"identities\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"username_id\" INTEGER,"
        "\"name\" TEXT,"
        "\"age\" INTEGER,"
        "\"status\" TEXT,"
        "\"birthday_id\" INTEGER,"
        "\"address_id\" INTEGER,"
        "\"profil_picture_id\" INTEGER)"
	    "PRIMARY KEY(\"id\"),";
    if (sqlite3_exec(db, identitiesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* usernamesTable =
        "CREATE TABLE \"usernames\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"identity_id\" INTEGER,"
        "\"name\" TEXT)"
	    "PRIMARY KEY(\"id\"),"
        "FOREIGN KEY(\"identity_id\") REFERENCES \"identities\"(\"id\") ON DELETE CASCADE";
    if (sqlite3_exec(db, usernamesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* contactsTable =
        "CREATE TABLE \"contacts\" ( "
        "\"identity_id\" INTEGER,"
        "\"type\" INTEGER,"
        "\"detail\" TEXT)"
        "FOREIGN KEY(\"identity_id\") REFERENCES \"identities\"(\"id\") ON DELETE CASCADE";
    if (sqlite3_exec(db, contactsTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* relationshipsTable =
        "CREATE TABLE \"relationships\" ( "
        "\"identity1_id\" INTEGER,"
        "\"identity2_id\" INTEGER,"
        "\"type\" INTEGER)"
        "FOREIGN KEY(\"identity1_id\") REFERENCES \"identities\"(\"id\") ON DELETE CASCADE"
        "FOREIGN KEY(\"identity2_id\") REFERENCES \"identities\"(\"id\") ON DELETE CASCADE";
    if (sqlite3_exec(db, relationshipsTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* datesTable =
        "CREATE TABLE \"dates\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"day\" TEXT,"
        "\"month\" TEXT,"
        "\"year\" TEXT)"
        "PRIMARY KEY(\"id\")";
    if (sqlite3_exec(db, datesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* picturesTable =
        "CREATE TABLE \"pictures\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"identity_id\" INTEGER,"
        "\"content\" BLOB)"
        "PRIMARY KEY(\"id\")"
        "FOREIGN KEY(\"identity_id\") REFERENCES \"identities\"(\"id\") ON DELETE CASCADE";
    if (sqlite3_exec(db, picturesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* addressesTable =
        "CREATE TABLE \"addresses\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"number\" INTEGER,"
        "\"street\" TEXT,"
        "\"city\" TEXT,"
        "\"country\" TEXT)"
        "PRIMARY KEY(\"id\")";
    if (sqlite3_exec(db, addressesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_close(db);
}
