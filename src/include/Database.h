#pragma once

#include <sqlite3.h>
#include <string>

class Database {
public:
    static void SetPath(std::string path);
    static void Create();

private:
    static std::string path;
    static sqlite3* db;
};
