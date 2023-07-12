#include "include/Database.h"
#include "include/Application.h"
#include "include/TextureManager.h"
#include "include/CatalogPage.h"

#include <iostream>
#include <algorithm>

std::string Database::path = "null";    
sqlite3* Database::db = nullptr;

void Database::SetPath(std::string path) {
    Database::path = path;
}

void Database::Create() {
    sqlite3_open(path.c_str(), &db);

    const char* identitiesTable =
        "CREATE TABLE IF NOT EXISTS \"identities\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"username_id\" INTEGER,"
        "\"name\" TEXT,"
        "\"age\" INTEGER,"
        "\"status\" INTEGER,"
        "\"birthday_id\" INTEGER,"
        "\"address_id\" INTEGER,"
        "\"profil_picture_id\" INTEGER,"
	    "PRIMARY KEY(\"id\"));";
    if (sqlite3_exec(db, identitiesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* usernamesTable =
        "CREATE TABLE IF NOT EXISTS \"usernames\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"identity_id\" INTEGER,"
        "\"name\" TEXT,"
	    "PRIMARY KEY(\"id\"));";
    if (sqlite3_exec(db, usernamesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* contactsTable =
        "CREATE TABLE IF NOT EXISTS \"contacts\" ( "
        "\"identity_id\" INTEGER,"
        "\"type\" INTEGER,"
        "\"detail\" TEXT);";
    if (sqlite3_exec(db, contactsTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* relationshipsTable =
        "CREATE TABLE IF NOT EXISTS \"relationships\" ( "
        "\"identity1_id\" INTEGER,"
        "\"identity2_id\" INTEGER,"
        "\"type\" INTEGER);";
    if (sqlite3_exec(db, relationshipsTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* datesTable =
        "CREATE TABLE IF NOT EXISTS \"dates\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"day\" TEXT,"
        "\"month\" TEXT,"
        "\"year\" TEXT,"
        "PRIMARY KEY(\"id\"));";
    if (sqlite3_exec(db, datesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* picturesTable =
        "CREATE TABLE IF NOT EXISTS \"pictures\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"identity_id\" INTEGER,"
        "\"content\" BLOB,"
        "PRIMARY KEY(\"id\"));";
    if (sqlite3_exec(db, picturesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* addressesTable =
        "CREATE TABLE IF NOT EXISTS \"addresses\" ( "
        "\"id\" INTEGER UNIQUE NOT NULL,"
        "\"number\" INTEGER,"
        "\"street\" TEXT,"
        "\"city\" TEXT,"
        "\"country\" TEXT,"
        "PRIMARY KEY(\"id\"));";
    if (sqlite3_exec(db, addressesTable, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_close(db);
}

void Database::LoadCatalog() {
    CatalogPage::Clear();

    sqlite3_open(path.c_str(), &db);

    const char* sql = "SELECT id, name, profil_picture_id FROM identities ORDER BY id ASC;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = (char*)sqlite3_column_text(stmt, 1);
        std::replace(name.begin(), name.end(), ' ', '\n');
        int profil_picture_id = sqlite3_column_int(stmt, 2);
        
        CatalogPage::Add(id, name.c_str(), GetPicture(profil_picture_id));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

SDL_Texture* Database::GetPicture(int id) {
    sqlite3_open(path.c_str(), &db);

    const char* sql = "SELECT content FROM pictures WHERE id=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW || rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return TextureManager::LoadTexture("./assets/catalog-unknown-identity.png");
    }

    const void* data = sqlite3_column_blob(stmt, 0);
    int dataSize = sqlite3_column_bytes(stmt, 0);

    sqlite3_finalize(stmt);

    SDL_RWops* rwOps = SDL_RWFromConstMem(data, dataSize);
    if (rwOps == nullptr) {
        sqlite3_close(db);
        return TextureManager::LoadTexture("./assets/catalog-unknown-identity.png");
    }

    SDL_Surface* surface = IMG_Load_RW(rwOps, 1);
    SDL_RWclose(rwOps);
    if (surface == nullptr) {
        sqlite3_close(db);
        return TextureManager::LoadTexture("./assets/catalog-unknown-identity.png");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Application::window->renderer, surface);
    if (texture == nullptr) {
        sqlite3_close(db);
        return TextureManager::LoadTexture("./assets/catalog-unknown-identity.png");
    }

    sqlite3_close(db);
    return texture;
}

std::string Database::GetName(int id) {
    sqlite3_open(path.c_str(), &db);

    const char* sql = "SELECT name FROM identities WHERE id=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW || rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "UNKNOWN";
    }

    std::string data = (const char*)sqlite3_column_text(stmt, 0);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return data;
}

std::vector<int> Database::GetBirthDay(int id) {
    sqlite3_open(path.c_str(), &db);

    const char* sql =
        "SELECT day, month, year FROM dates"
        "JOIN identities ON dates.id = identities.birthday_id"
        "WHERE identities.id = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW || rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return {-1, -1, -1};
    }

    std::vector<int> data = {sqlite3_column_int(stmt, 0), sqlite3_column_int(stmt, 1), sqlite3_column_int(stmt, 2)};

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return data;
}

int Database::GetAge(int id) {
    sqlite3_open(path.c_str(), &db);

    const char* sql = "SELECT age FROM identities WHERE id=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW || rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    int data = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return data;
}

int Database::GetStatus(int id) {
    sqlite3_open(path.c_str(), &db);

    const char* sql = "SELECT status FROM identities WHERE id=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW || rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    int data = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return data;
}
