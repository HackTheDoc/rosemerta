#pragma once

#include "error.h"
#include "entity.h"

#include <filesystem>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <set>

namespace fs = std::filesystem;

class Database {
public:
    static error::code error;

    static const char* err_msg();

    static void SetPath(fs::path p);

    static bool Exist();

    static bool Create();

    static int Size();

    /// @brief Return a list of Person on a certain page
    /// @param page page index to display (default:0)
    /// @return a vector with the list of persons
    static std::vector<std::pair<int, std::string>> List(int page = 0);

    /// @brief List every id of the database
    /// @return
    static std::vector<int> ListIDs();

    /// @brief Insert a new element int the database (with just the name and/or username)
    /// @param firsname
    /// @param lastname
    /// @param username
    /// @return id of the inserted item (-1 if failed to insert);
    static int Insert(const std::string& firstname, const std::string& lastname, const std::string& username);

    static bool Delete(const int id);

    /// @brief Find the ID associated with a given name or username
    /// @param name name or username (or part of it at least)
    /// @return a set containing possibly associated IDs
    static std::set<int> Find(std::string name);

    static Entity Get(const int id);

    static bool SetFirstname(const int id, const std::string& v);
    static bool SetLastname(const int id, const std::string& v);
    static bool SetUsername(const int id, const std::string& v);
    static bool SetAge(const int id, const int v);
    static bool SetBirthday(const int id, const std::string& v);
    static bool SetStatus(const int id, const Entity::Status v);

private:
    static fs::path path;
    static sqlite3* db;

    static bool ExistID(const int id);
};
