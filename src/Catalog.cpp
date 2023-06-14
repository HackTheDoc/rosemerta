#include "include/Catalog.h"
#include "include/Application.h"
#include "include/Identity.h"

#include <iostream>
#include <sqlite3.h>

Catalog::Catalog() {}

Catalog::~Catalog() {}

void Catalog::load() {
    loadIdentities();
    loadContacts();
    loadLocations();
}

void Catalog::loadIdentities() {
    std::string sql = "SELECT * FROM identity;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(Application::database, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing SQL statement: " << sqlite3_errmsg(Application::database);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Identity* id = new Identity();
        id->setID(sqlite3_column_int(stmt, 0));
        id->setUsername((const char*)sqlite3_column_text(stmt, 1));
        id->setName((const char*)sqlite3_column_text(stmt, 2));
        id->setLastname((const char*)sqlite3_column_text(stmt, 3));
        id->setAge(sqlite3_column_int(stmt, 4));
        id->setBirthday((const char*)sqlite3_column_text(stmt, 5));
        id->setStatus(sqlite3_column_int(stmt, 6));
        items[id->getID()] = id;
    }
}

void Catalog::loadContacts() {
    // TODO
}

void Catalog::loadLocations() {
    // TODO
}

Identity* Catalog::at(const int& id) {
    try {
        return items.at(id);
    }
    catch(const std::exception& e)
    {
        std::cerr << "ERROR: impossible to find card with id \"" << id << "\"" << std::endl;
        throw(e);
    }
    
    return nullptr;
}

void Catalog::clear() {
    for (auto i : items) {
        delete i.second;
    }
    items.clear();
}

std::unordered_map<int, Identity*>::iterator Catalog::begin() noexcept {
    return items.begin();
}

std::unordered_map<int, Identity*>::iterator Catalog::end() noexcept {
    return items.end();
}

std::unordered_map<int, Identity*>::const_iterator Catalog::begin() const noexcept {
    return items.cbegin();
}

std::unordered_map<int, Identity*>::const_iterator Catalog::end() const noexcept {
    return items.cend();
}

std::ostream& operator<<(std::ostream& stream, const Catalog& c) {
    stream << " ---------- Catalog ----------" << std::endl;
    
    for (auto i : c.items) {
        stream << *i.second << std::endl;
    }
    
    stream << " -----------------------------";

    return stream;
}
