#include "include/Application.h"
#include "include/Catalog.h"

#include <iostream>
#include <cstdlib>

sqlite3* Application::database = nullptr;
Catalog* Application::catalog = nullptr;

Application::Application() {}

Application::~Application() {}

void Application::init() {
    sqlite3_open("./database.db", &database);
    
    catalog = new Catalog();
    catalog->load();

    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void Application::run() {
    std::cout << *catalog << std::endl;
}

void Application::kill() {
    sqlite3_close(database);
    database = nullptr;

    catalog->clear();
    delete catalog;
    catalog = nullptr;
}
