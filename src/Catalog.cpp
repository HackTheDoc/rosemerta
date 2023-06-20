#include "include/Catalog.h"
#include "include/Application.h"
#include "include/Identity.h"

#include <iostream>
#include <sqlite3.h>
#include <stack>

std::set<std::pair<int, Catalog::ChangingType>> Catalog::changelog = {};

Catalog::Catalog() : loadedCapacity(0) {}

Catalog::~Catalog() {}

void Catalog::load() {
    loadIdentities();
    loadContacts();
    loadLocations();

    loadedCapacity = items.size();
}

void Catalog::loadIdentities() {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::string sql = "SELECT * FROM identity;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing SQL statement: " << sqlite3_errmsg(db);
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
        id->addNotes((const char*)sqlite3_column_text(stmt, 7));
        items[id->getID()] = id;
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Catalog::loadContacts() {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    for (auto item : items) {
        std::string sql = "SELECT * FROM contact WHERE owner = ? ORDER BY type DESC;";
        sqlite3_stmt* stmt;
        
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, item.first);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            item.second->addContact(
                (Contact::Type)sqlite3_column_int(stmt, 1),
                (const char*)sqlite3_column_text(stmt, 2)
            );
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
}

void Catalog::loadLocations() {
    // TODO
}

void Catalog::save() {
    for (auto change : changelog) {
        switch (change.second) {
        case ChangingType::USERNAME:
            updateUsername(change.first);
            break;
        case ChangingType::NAME:
            updateName(change.first);
            break;
        case ChangingType::LASTNAME:
            updateLastname(change.first);
            break;
        case ChangingType::AGE:
            updateAge(change.first);
            break;
        case ChangingType::BIRTHDAY:
            updateBirthday(change.first);
            break;
        case ChangingType::STATUS:
            updateStatus(change.first);
            break;
        case ChangingType::NOTES:
            updateNotes(change.first);
            break;
        case ChangingType::ERASE:
            eraseID(change.first);
            break;
        case ChangingType::UPDATE_CONTACT:
            updateContacts(change.first);
            break;
        case ChangingType::NEW_IDENTITY:
            saveIdentity(change.first);
            break;
        default:
            break;
        }
    }

    changelog.clear();
}

void Catalog::updateUsername(int id) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::string v = items.at(id)->getUsername();
    std::string sql = "UPDATE identity SET username = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, v.c_str(), v.size(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "\033[31m" << "Failed to update: " << "\033[0m";
        std::cout << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Catalog::updateName(int id) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::string v = items.at(id)->getName();
    std::string sql = "UPDATE identity SET name = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, v.c_str(), v.size(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "\033[31m" << "Failed to update: " << "\033[0m";
        std::cout << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Catalog::updateLastname(int id) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::string v = items.at(id)->getLastname();
    std::string sql = "UPDATE identity SET lastname = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, v.c_str(), v.size(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "\033[31m" << "Failed to update: " << "\033[0m";
        std::cout << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Catalog::updateAge(int id) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    int v = items.at(id)->getAge();
    std::string sql = "UPDATE identity SET age = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, v);
    sqlite3_bind_int(stmt, 2, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "\033[31m" << "Failed to update: " << "\033[0m";
        std::cout << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Catalog::updateBirthday(int id) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::string v = items.at(id)->getBirthday();
    std::string sql = "UPDATE identity SET birthday = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, v.c_str(), v.size(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "\033[31m" << "Failed to update: " << "\033[0m";
        std::cout << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Catalog::updateStatus(int id) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    int v = items.at(id)->getStatus();
    std::string sql = "UPDATE identity SET status = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, v);
    sqlite3_bind_int(stmt, 2, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "\033[31m" << "Failed to update: " << "\033[0m";
        std::cout << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Catalog::updateNotes(int id) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::string v = items.at(id)->getNotes();
    std::string sql = "UPDATE identity SET notes = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, v.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "\033[31m" << "Failed to update: " << "\033[0m";
        std::cout << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void Catalog::eraseID(int id) {
    Identity* i = deletedItems.at(id);
    i->clear();

    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::string deleteContactsQuery = "DELETE FROM contact WHERE owner = ?;";
    sqlite3_stmt* deleteContactsStatement;
    sqlite3_prepare_v2(db, deleteContactsQuery.c_str(), -1, &deleteContactsStatement, nullptr);
    sqlite3_bind_int(deleteContactsStatement, 1, id);
    if (sqlite3_step(deleteContactsStatement) != SQLITE_DONE)
        Application::Error("unable to clear identity "+std::to_string(id));
    sqlite3_finalize(deleteContactsStatement);

    std::string deleteIdQuery = "DELETE FROM identity WHERE id = ?;";
    sqlite3_stmt* deleteIdStatement;
    sqlite3_prepare_v2(db, deleteIdQuery.c_str(), -1, &deleteIdStatement, nullptr);
    sqlite3_bind_int(deleteIdStatement, 1, id);
    if (sqlite3_step(deleteIdStatement) != SQLITE_DONE)
        Application::Error("unable to clear identity "+std::to_string(id));
    sqlite3_finalize(deleteIdStatement);

    sqlite3_close(db);
}

void Catalog::updateContacts(int id) {
    Identity* i = at(id);
    if (i == nullptr) return;
    std::stack<Contact*>* contacts = i->getContacts();
    Contact* c;

    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::string deleteQuery = "DELETE FROM contact WHERE owner = ?;";
    sqlite3_stmt* deleteStatement;
    sqlite3_prepare_v2(db, deleteQuery.c_str(), -1, &deleteStatement, nullptr);
    sqlite3_bind_int(deleteStatement, 1, id);
    if (sqlite3_step(deleteStatement) != SQLITE_DONE)
        Application::Error("unable to clear old contacts");
    sqlite3_finalize(deleteStatement);

    while(!contacts->empty()) {
        c = contacts->top();
        contacts->pop();

        std::string insertQuery = "INSERT INTO contact (owner, type, detail) VALUES (?,?,?);";
        sqlite3_stmt* insertStatement;
        sqlite3_prepare_v2(db, insertQuery.c_str(), -1, &insertStatement, nullptr);

        sqlite3_bind_int(insertStatement, 1, id);
        sqlite3_bind_int(insertStatement, 2, c->getType());
        sqlite3_bind_text(insertStatement, 3, c->getDetails().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(insertStatement) != SQLITE_DONE) {
            Application::Error(sqlite3_errmsg(db));
        }

        sqlite3_finalize(insertStatement);
    }
    sqlite3_close(db);
}

void Catalog::saveIdentity(int id) {
    Identity* i = at(id);
    if (i == nullptr) return;

    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::string insertQuery = "INSERT INTO identity (id, username, name, lastname, age, birthday, status, notes) VALUES (?,?,?,?,?,?,?,?);";
    sqlite3_stmt* insertStatement;
    sqlite3_prepare_v2(db, insertQuery.c_str(), -1, &insertStatement, nullptr);

    sqlite3_bind_int(insertStatement , 1, id);
    sqlite3_bind_text(insertStatement, 2, i->getUsername().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertStatement, 3, i->getName().c_str()    , -1, SQLITE_STATIC);
    sqlite3_bind_text(insertStatement, 4, i->getLastname().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(insertStatement , 5, i->getAge());
    sqlite3_bind_text(insertStatement, 6, i->getBirthday().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(insertStatement , 7, i->getStatus());
    sqlite3_bind_text(insertStatement, 8, i->getNotes().c_str()   , -1, SQLITE_STATIC);

    if (sqlite3_step(insertStatement) != SQLITE_DONE) {
        Application::Error(sqlite3_errmsg(db));
    }

    sqlite3_finalize(insertStatement);
    sqlite3_close(db);
}

void Catalog::insert(Identity* i) {
    int id = nextFreeID();
    i->setID(id);
    items[id] = i;
}

Identity* Catalog::at(const int& id) {
    try {
        return items.at(id);
    }
    catch(const std::exception& e)
    {
        Application::Error("unknown id "+std::to_string(id));
    }
    
    return nullptr;
}

std::set<int> Catalog::findID(std::string name) {
    std::set<int> buffer = findByUsername(name);

    std::set<int> temp = findByName(name);
    for (int e : temp) {
        buffer.insert(e);
    }

    temp = findByFirstname(name);
    for (int e : temp) {
        buffer.insert(e);
    }

    temp = findByLastname(name);
    for (int e : temp) {
        buffer.insert(e);
    }
    
    return buffer;
}

std::set<int> Catalog::findByUsername(std::string username) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::set<int> buffer;

    std::string sql = "SELECT * FROM identity WHERE username = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        buffer.insert(id);
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return buffer;
}

std::set<int> Catalog::findByName(std::string name) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::set<int> buffer;

    std::string sql = "SELECT * FROM identity WHERE name = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        buffer.insert(id);
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return buffer;
}

std::set<int> Catalog::findByLastname(std::string lastname) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::set<int> buffer;

    std::string sql = "SELECT * FROM identity WHERE name = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, (lastname).c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        buffer.insert(id);
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return buffer;
}

std::set<int> Catalog::findByFirstname(std::string firstname) {
    sqlite3* db;
    sqlite3_open(Application::database.c_str(), &db);

    std::set<int> buffer;

    std::string sql = "SELECT * FROM identity WHERE name = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, (firstname+"%").c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        buffer.insert(id);
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return buffer;
}

int Catalog::count(const int& id) {
    return items.count(id);
}

void Catalog::erase(const int& id) {
    Identity* i = at(id);
    if (i == nullptr) {
        Application::Error("unknown id '"+std::to_string(id)+"'");
        return;
    }

    items.erase(id);
    deletedItems.insert(std::make_pair(id, i));
    changelog.insert(std::make_pair(id, ERASE));
}

void Catalog::removeContact(const int& id, std::string type, int index) {
    Identity* i = this->at(id);
    if (i == nullptr) {
        Application::Error("unknown id '"+std::to_string(id)+"'");
        return;
    }
    i->removeContact(type, index);
    changelog.insert(std::make_pair(id, UPDATE_CONTACT));
    save();
}

void Catalog::removeContacts(const int& id) {
    Identity* i = this->at(id);
    if (i == nullptr) {
        Application::Error("unknown id '"+std::to_string(id)+"'");
        return;
    }
    i->removeContacts();
    changelog.insert(std::make_pair(id, UPDATE_CONTACT));

    save();
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

int Catalog::nextFreeID() {
    int id = 1;
    while (id <= (int)items.size() && items[id]->getID()) id++;
    return id;
}

void Catalog::display() {
    std::cout << " ---------- Catalog ----------";
    
    for (auto i : items) {
        std::cout << std::endl;
        i.second->resume();
    }
    if (items.size() == 0) std::cout << std::endl;
    std::cout << " -----------------------------";
    std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& stream, const Catalog& c) {
    stream << " ---------- Catalog ----------";
    
    for (auto i : c.items) {
        stream << std::endl;
        stream << *i.second << std::endl;
    }
    if (c.items.size() == 0) stream << std::endl;
    stream << " -----------------------------";

    return stream;
}
