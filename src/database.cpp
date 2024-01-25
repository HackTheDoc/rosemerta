#include "include/database.h"
#include "include/application.h"

#include <algorithm>

#ifdef _WIN32
#include <dirent.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0777)
#endif

#include <iostream>

error::code Database::error = error::code::NONE;
fs::path Database::path = "/tmp/.rosemerta.db";
sqlite3* Database::db = nullptr;

const char* Database::err_msg() {
    return sqlite3_errmsg(db);
}

std::string naming(std::string f, std::string l, std::string u) {
    if ((f == "" || l == "") && u != "")
        return u;

    if (f == "" && l == "")
        return "";

    return f + " " + l;
}


/* ----- DATABASE ----- */

void Database::SetPath(fs::path p) {
    path = p;
}

bool Database::Exist() {
    // check existence
    if (!std::filesystem::exists(path)) {
        error = error::code::DATABASE_DOES_NOT_EXIST;
        return false;
    }

    // check extension
    if (std::filesystem::path(path).extension() != ".db") {
        error = error::code::DATABASE_DOES_NOT_EXIST;
        return false;
    }

    error = error::code::NONE;
    return true;
}

bool Database::Create() {
    if (path != "/tmp/rosemerta.db")
        MKDIR(path.c_str());
    path += "database-" + Application::version + ".db";

    sqlite3_open(path.c_str(), &db);

    const char* tableIdentities =
        "CREATE TABLE IF NOT EXISTS 'identities' ( "
        "'id' INTEGER UNIQUE NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "'firstname' TEXT,"
        "'lastname' TEXT,"
        "'nickname' TEXT,"
        "'age' INTEGER,"
        "'birthday' TEXT,"
        "'status' INTEGER"
        ");";
    if (sqlite3_exec(db, tableIdentities, nullptr, nullptr, nullptr) != SQLITE_OK) {
        error = error::code::CANNOT_CREATE_DATABASE;
        return false;
    }

    const char* tableContacts =
        "CREATE TABLE IF NOT EXISTS 'contacts' ( "
        "'id' INTEGER UNIQUE NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "'owner_id' INTEGER,"
        "'type' INTEGER,"
        "'username' TEXT,"
        "'password' TEXT"
        ");";
    if (sqlite3_exec(db, tableContacts, nullptr, nullptr, nullptr) != SQLITE_OK) {
        error = error::code::CANNOT_CREATE_DATABASE;
        return false;
    }

    const char* tableNotes =
        "CREATE TABLE IF NOT EXISTS 'notes' ( "
        "'id' INTEGER UNIQUE NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "'owner_id' INTEGER,"
        "'text' TEXT"
        ");";
    if (sqlite3_exec(db, tableNotes, nullptr, nullptr, nullptr) != SQLITE_OK) {
        error = error::code::CANNOT_CREATE_DATABASE;
        return false;
    }

    const char* tableAddresses =
        "CREATE TABLE IF NOT EXISTS 'addresses' ( "
        "'owner_id' INTEGER,"
        "'title' TEXT,"
        "'detail' TEXT"
        ");";
    if (sqlite3_exec(db, tableAddresses, nullptr, nullptr, nullptr) != SQLITE_OK) {
        error = error::code::CANNOT_CREATE_DATABASE;
        return false;
    }

    error = error::code::NONE;
    return true;
}

int Database::Size() {
    int size = 0;

    sqlite3_open(path.c_str(), &db);

    const char* query = "SELECT COUNT(*) FROM identities;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        size = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return size;
}

std::vector<std::pair<int, std::string>> Database::List(int page) {
    std::vector<std::pair<int, std::string>> buffer;

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::CANNOT_OPEN_DATABASE;
        return buffer;
    }

    const char* query = "SELECT id, firstname, lastname, nickname FROM identities ORDER BY id ASC";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    // 9 elements per pages
    int i = 0;
    int imax = page * 9 + 9;
    int imin = page * 9;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < imax) {
        if (i < imin) i++;
        else {
            int id = sqlite3_column_int(stmt, 0);
            std::string firstname = (const char*)sqlite3_column_text(stmt, 1);
            std::string lastname = (const char*)sqlite3_column_text(stmt, 2);
            std::string nickname = (const char*)sqlite3_column_text(stmt, 3);

            buffer.push_back(std::make_pair(id, naming(firstname, lastname, nickname)));

            i++;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return buffer;
}

std::vector<int> Database::ListIDs() {
    std::vector<int> buffer;

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return buffer;
    }

    const char* query = "SELECT id FROM identities ORDER BY id ASC";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW)
        buffer.push_back(sqlite3_column_int(stmt, 0));

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return buffer;
}

int Database::Insert(const std::string& firstname, const std::string& lastname, const std::string& nickname) {
#pragma region INSERTION
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return -1;
    }

    const char* query = "INSERT INTO identities (firstname, lastname, nickname, age, birthday, status) VALUES (?,?,?,?,?,?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::CANNOT_PREPARE_QUERY;
        return -1;
    }

    sqlite3_bind_text(stmt, 1, firstname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lastname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, nickname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, -1);
    sqlite3_bind_text(stmt, 5, "", -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, Entity::Status::UNKNOWN);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error = error::code::SQL;
        return -1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

#pragma endregion 

#pragma region ID PARSER
    rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return -1;
    }

    query = "SELECT id FROM identities ORDER BY id DESC LIMIT 1;";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::CANNOT_PREPARE_QUERY;
        return -1;
    }

    int id = -1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

#pragma endregion

    error = error::code::NONE;
    return id;
}

bool Database::Delete(const int id) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return false;
    }

    const char* query = "DELETE FROM identities WHERE id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::CANNOT_PREPARE_QUERY;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

std::set<int> Database::Find(std::string name) {
    std::set<int> buffer;

#pragma region USERNAME SEARCH
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
    }

    const char* query = "SELECT id FROM identities WHERE username = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        buffer.insert(id);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
#pragma endregion

#pragma region FIRSTNAME SEARCH
    rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
    }

    query = "SELECT id FROM identities WHERE firstname = ?;";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        buffer.insert(id);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
#pragma endregion

#pragma region LASTNAME SEARCH
    rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
    }

    query = "SELECT id FROM identities WHERE lastname = ?;";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        buffer.insert(id);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
#pragma endregion

#pragma region FULL NAME SEARCH
    std::string firstname, lastname;
    std::stringstream ss(name);
    std::string word;
    while (getline(ss, word, ' ')) {
        if (firstname.empty())
            firstname = word;
        else if (lastname.empty())
            lastname = word;
    }

    rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
    }

    query = "SELECT id FROM identities WHERE firstname = ? AND lastname = ?;";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, firstname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lastname.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        buffer.insert(id);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
#pragma endregion

    return buffer;
}

Entity Database::Get(const int id) {
    /*
    struct Person {
        std::string formatted_name;
        std::string firstname;
        std::string lastname;
        std::string username;
        int age;
        std::string birthday;
        Status status;
    };
    */
    Entity p;

    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return p;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return p;
    }

    const char* query = "SELECT * FROM identities WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW || rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        error = error::code::SQL;
        return p;
    }

    p.firstname = (const char*)sqlite3_column_text(stmt, 1);
    p.lastname = (const char*)sqlite3_column_text(stmt, 2);
    p.username = (const char*)sqlite3_column_text(stmt, 3);
    p.age = sqlite3_column_int(stmt, 4);
    p.birthday = (const char*)sqlite3_column_text(stmt, 5);
    p.status = (Entity::Status)sqlite3_column_int(stmt, 6);

    p.formatted_name = naming(p.firstname, p.lastname, p.username);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    p.addresses = GetAddresses(id);
    p.contacts = GetContacts(id);

#pragma region GET NOTES   
    rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return p;
    }

    query = "SELECT * FROM notes WHERE owner_id = ?;";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string n = (const char*)sqlite3_column_text(stmt, 2);
        p.notes.push_back(n);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
#pragma endregion 

    error = error::code::NONE;
    return p;
}

std::vector<Address> Database::GetAddresses(const int id) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return {};
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return {};
    }

    const char* query = "SELECT * FROM addresses WHERE owner_id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, id);

    std::vector<Address> addresses;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Address a{
            (const char*)sqlite3_column_text(stmt, 1),
            (const char*)sqlite3_column_text(stmt, 2)
        };
        addresses.push_back(a);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return addresses;
}

std::vector<Contact> Database::GetContacts(const int id) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return {};
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return {};
    }

    const char* query = "SELECT * FROM contacts WHERE owner_id = ? ORDER BY type ASC;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, id);

    std::vector<Contact> contacts;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Contact c{
            to_contact_type(sqlite3_column_int(stmt, 2)),
            (const char*)sqlite3_column_text(stmt, 3),
            (const char*)sqlite3_column_text(stmt, 4)
        };
        contacts.push_back(c);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return contacts;
}

bool Database::SetFirstname(const int id, const std::string& v) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return false;
    }

    const char* query = "UPDATE identities SET firstname = ? WHERE id = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, v.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

bool Database::SetLastname(const int id, const std::string& v) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return false;
    }

    const char* query = "UPDATE identities SET lastname = ? WHERE id = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, v.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

bool Database::SetNickname(const int id, const std::string& v) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return false;
    }

    const char* query = "UPDATE identities SET nickname = ? WHERE id = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, v.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

bool Database::SetAge(const int id, int v) {
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return false;
    }

    const char* query = "UPDATE identities SET age = ? WHERE id = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, v);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

bool Database::SetBirthday(const int id, const std::string& v) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return false;
    }

    const char* query = "UPDATE identities SET birthday = ? WHERE id = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, v.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

bool Database::SetStatus(const int id, const Entity::Status v) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return false;
    }

    const char* query = "UPDATE identities SET status = ? WHERE id = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, v);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

bool Database::AddAddress(const int id, const std::string& title, const std::string& address) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }

    if (ExistAddress(id, title)) {
        error = error::code::EXISTING_ADDRESS;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK)
        return false;

    const char* query = "INSERT INTO addresses (owner_id, title, detail) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, address.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

bool Database::SetAddress(const int id, const std::string& title, const std::string& address) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return false;
    }

    const char* query = "UPDATE addresses SET detail = ? WHERE owner_id = ? AND title = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_text(stmt, 3, title.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

std::string Database::RemoveAddress(const int id, const std::string& title) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return "";
    }

    std::string address;

#pragma region FETCH ADDRESS
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return address;
    }

    const char* query = "SELECT * FROM addresses WHERE owner_id = ? AND title = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return address;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
        address = (const char*)sqlite3_column_text(stmt, 2);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
#pragma endregion

#pragma region REMOVE
    rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return address;
    }

    query = "DELETE FROM addresses WHERE owner_id = ? AND title = ?;";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::SQL;
        return address;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error = error::code::SQL;
        return address;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
#pragma endregion

    error = error::code::NONE;
    return address;
}

bool Database::AddContact(const int id, const Contact::Type type, const std::string& username, const std::string& password) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }


    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::INVALID_ID;
        return false;
    }


    const char* query = "INSERT INTO contacts (owner_id, type, username, password) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        error = error::code::SQL;
        return false;
    }


    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_int(stmt, 2, type);
    sqlite3_bind_text(stmt, 3, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

Contact Database::RemoveContact(const int id, const Contact::Type type, const int index) {
    Contact contact;

    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return contact;
    }

    int contactID = -1;

#pragma region FETCH CONTACT REAL ID
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return contact;
    }

    const char* query = "SELECT * FROM contacts WHERE owner_id = ? AND type = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::SQL;
        return contact;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_int(stmt, 2, type);

    int i = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (i == index) {
            contactID = sqlite3_column_int(stmt, 0);
            contact.u = (const char*)sqlite3_column_text(stmt, 3);
            contact.p = (const char*)sqlite3_column_text(stmt, 4);
        }
        else i++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
#pragma endregion

    error = error::code::INVALID_ID;
    if (contactID == -1) return contact;

#pragma region REMOVE CONTACT
    rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return contact;
    }

    query = "DELETE FROM contacts WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::SQL;
        return contact;
    }

    sqlite3_bind_int(stmt, 1, contactID);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error = error::code::SQL;
        return contact;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
#pragma endregion

    error = error::code::NONE;
    return contact;
}

bool Database::AddNote(const int id, const std::string& text) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::INVALID_ID;
        return false;
    }

    const char* query = "INSERT INTO notes (owner_id, text) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, text.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error = error::code::SQL;
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    error = error::code::NONE;
    return true;
}

std::string Database::RemoveNote(int id, int index) {
    if (!ExistID(id)) {
        error = error::code::INVALID_ID;
        return "";
    }

    int noteID = -1;
    std::string note;

#pragma region FETCH NOTE REAL ID
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return "";
    }

    const char* query = "SELECT * FROM notes WHERE owner_id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::SQL;
        return "";
    }

    sqlite3_bind_int(stmt, 1, id);

    int i = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (i == index) {
            noteID = sqlite3_column_int(stmt, 0);
            note = (const char*)sqlite3_column_text(stmt, 2);
        }
        else i++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    error = error::code::NONE;
#pragma endregion

    if (noteID == -1) return "";

#pragma region REMOVE NOTE
    rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        error = error::code::CANNOT_OPEN_DATABASE;
        return "";
    }

    query = "DELETE FROM notes WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error = error::code::SQL;
        return "";
    }

    sqlite3_bind_int(stmt, 1, noteID);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error = error::code::SQL;
        return "";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    error = error::code::NONE;
#pragma endregion

    return note;
}

bool Database::ExistID(const int id) {
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK)
        return false;

    const char* query = "SELECT COUNT(*) FROM identities WHERE id = ?";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    int n = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        n = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return n > 0;
}

bool Database::ExistAddress(const int id, const std::string& title) {
    if (!ExistID(id)) {
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK)
        return false;

    const char* query = "SELECT COUNT(*) FROM addresses WHERE owner_id = ? AND title = ?";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);

    int n = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        n = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return n > 0;
}
