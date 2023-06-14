#pragma once

#include <sqlite3.h>

class Catalog;

class Application {
public:
    static sqlite3* database;
    static Catalog* catalog;

    Application();
    ~Application();

    void init();
    void run();
    void kill();

private:

};
