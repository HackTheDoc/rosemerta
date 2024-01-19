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

fs::path Database::path = "/tmp/.rosemerta.db";
sqlite3* Database::db = nullptr;

void Database::SetPath(fs::path p) {
    path = p;
}

bool Database::Exist() {
    // check existence
    if (!std::filesystem::exists(path))
        return false;

    // check extension
    if (std::filesystem::path(path).extension() != ".db")
        return false;

    return true;
}

bool Database::Create() {
    if (path != "/tmp/rosemerta.db")
        MKDIR(path.c_str());
    path += "database-"+ Application::version + ".db";

    sqlite3_open(path.c_str(), &db);

    const char* identitiesTable =
        "CREATE TABLE IF NOT EXISTS 'identities' ( "
        "'id' INTEGER UNIQUE NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "'id' INTEGER UNIQUE NOT NULL,"
        "'username_id' INTEGER,"
        "'name' TEXT,"
        "'age' INTEGER,"
        "'status' INTEGER,"
        "'birthday_id' INTEGER,"
        "'address_id' INTEGER,"
        "'profil_picture_id' INTEGER,"
	    "PRIMARY KEY('id'));";
    if (sqlite3_exec(db, identitiesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        return false;
    }

    const char* usernamesTable =
        "CREATE TABLE IF NOT EXISTS 'usernames' ( "
        "'id' INTEGER UNIQUE NOT NULL,"
        "'identity_id' INTEGER,"
        "'name' TEXT,"
	    "PRIMARY KEY('id'));";
    if (sqlite3_exec(db, usernamesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        return false;
    }

    const char* contactsTable =
        "CREATE TABLE IF NOT EXISTS 'contacts' ( "
        "'identity_id' INTEGER,"
        "'type' INTEGER,"
        "'detail' TEXT);";
    if (sqlite3_exec(db, contactsTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        return true;
    }

    const char* relationshipsTable =
        "CREATE TABLE IF NOT EXISTS 'relationships' ( "
        "'identity1_id' INTEGER,"
        "'identity2_id' INTEGER,"
        "'type' INTEGER);";
    if (sqlite3_exec(db, relationshipsTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        return true;
    }

    const char* datesTable =
        "CREATE TABLE IF NOT EXISTS 'dates' ( "
        "'id' INTEGER UNIQUE NOT NULL,"
        "'day' TEXT,"
        "'month' TEXT,"
        "'year' TEXT,"
        "PRIMARY KEY('id'));";
    if (sqlite3_exec(db, datesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        return true;
    }

    const char* picturesTable =
        "CREATE TABLE IF NOT EXISTS 'pictures' ( "
        "'id' INTEGER UNIQUE NOT NULL,"
        "'identity_id' INTEGER,"
        "'content' BLOB,"
        "PRIMARY KEY('id'));";
    if (sqlite3_exec(db, picturesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        return true;
    }

    const char* addressesTable =
        "CREATE TABLE IF NOT EXISTS 'addresses' ( "
        "'id' INTEGER UNIQUE NOT NULL,"
        "'number' INTEGER,"
        "'street' TEXT,"
        "'city' TEXT,"
        "'country' TEXT,"
        "PRIMARY KEY('id'));";
    if (sqlite3_exec(db, addressesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        return true;
    }
    sqlite3_close(db);

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

/*
void Database::Copy(std::string p, int v) {
    switch (v) {
    case 1:
        Copy_V1(p);
        break;
    default:
        Application::Error("nothing to update");
        break;
    }
}

std::vector<std::string> Database::List(bool chatroom, bool forum, bool library, bool scam, bool wiki) {
    std::vector<std::string> buffer;

    sqlite3_open(path.c_str(), &db);

    std::string query = "SELECT name FROM links ";
    if (chatroom)
        query += "WHERE chatroom = ? ";
    else if (forum)
        query += "WHERE forum = ? ";
    else if (library)
        query += "WHERE library = ? ";
    else if (scam)
        query += "WHERE scam = ? ";
    else if (wiki)
        query += "WHERE wiki = ? ";
        
    query += "ORDER BY name ASC;";
    
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (chatroom)
        sqlite3_bind_int(stmt, 1, chatroom);
    else if (forum)
        sqlite3_bind_int(stmt, 1, forum);
    else if (library)
        sqlite3_bind_int(stmt, 1, library);
    else if (scam)
        sqlite3_bind_int(stmt, 1, scam);
    else if (wiki)
        sqlite3_bind_int(stmt, 1, wiki);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        buffer.push_back((const char*)sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return buffer;
}


std::string Database::Get(std::string name) {
    sqlite3_open(path.c_str(), &db);

    const char* query = "SELECT link, scam FROM links WHERE name = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW || rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    std::string data = (const char*)sqlite3_column_text(stmt, 0);
    
    int scam = sqlite3_column_int(stmt, 1);
    if (scam) data.insert(data.begin(), '$'); 

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return data;
}

bool Database::Insert(std::string name, std::string link, bool chatroom, bool forum, bool library, bool scam, bool wiki) {
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK)
        return false;

    const char* query = "INSERT INTO links (name, link, chatroom, forum, library, scam, wiki) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    } 

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, link.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int( stmt, 3, chatroom);
    sqlite3_bind_int( stmt, 4, forum);
    sqlite3_bind_int( stmt, 5, library);
    sqlite3_bind_int( stmt, 6, scam);
    sqlite3_bind_int( stmt, 7, wiki);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

bool Database::Remove(std::string name) {
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK)
        return false;
    
    const char* query = "DELETE FROM links WHERE name = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    } 

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

void Database::SetLink(std::string name, std::string link) {
    sqlite3_open(path.c_str(), &db);

    const char* query = "UPDATE links SET link = ? WHERE name = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, link.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        Application::Error("failed to update the link");
    }
    
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Database::SetChatroom(std::string name, bool value) {
    sqlite3_open(path.c_str(), &db);

    const char* query = "UPDATE links SET chatroom = ? WHERE name = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_int( stmt, 1, value);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        Application::Error("failed to update the chatroom flag");
    }
    
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Database::SetForum(std::string name, bool value) {
    sqlite3_open(path.c_str(), &db);

    const char* query = "UPDATE links SET forum = ? WHERE name = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_int( stmt, 1, value);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        Application::Error("failed to update the forum flag");
    }
    
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Database::SetLibrary(std::string name, bool value) {
    sqlite3_open(path.c_str(), &db);

    const char* query = "UPDATE links SET library = ? WHERE name = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_int( stmt, 1, value);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        Application::Error("failed to update the library flag");
    }
    
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Database::SetScam(std::string name, bool value) {
    sqlite3_open(path.c_str(), &db);

    const char* query = "UPDATE links SET scam = ? WHERE name = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_int( stmt, 1, value);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        Application::Error("failed to update the scam flag");
    }
    
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Database::SetWiki(std::string name, bool value) {
    sqlite3_open(path.c_str(), &db);

    const char* query = "UPDATE links SET wiki = ? WHERE name = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    sqlite3_bind_int( stmt, 1, value);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        Application::Error("failed to update the wiki flag");
    }
    
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}


void Database::Copy_V1(std::string p) {
    if (!HaveLinksTable(p)) {
        Application::Error("located a database at '"+p+"\" but table 'links' not found");
        return true;
    }

    sqlite3_open(p.c_str(), &db);

    const char* query = "SELECT name, link, scam FROM links";
    sqlite3_stmt* stmt;
    sqlite3_prepare(db, query, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = (const char*)sqlite3_column_text(stmt, 0);
        std::string link = (const char*)sqlite3_column_text(stmt, 1);
        int scam = sqlite3_column_int(stmt, 2);

        if (Get(name) != "")
            continue;
        else
            Insert(name, link, false, false, false, scam, false);
    }

    sqlite3_close(db);
}
*/