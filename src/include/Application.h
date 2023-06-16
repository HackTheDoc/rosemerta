#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>

class Catalog;

class Application {
public:
    enum Command {
        UNKNOWN,
        HELP,
        EXIT,
        CLEAR,
        SAVE,
        CATALOG,
        CONTACTS,
        ID,
        SET
    };

    static std::string database;
    static Catalog* catalog;
    static bool isRunning;

    Application();
    ~Application();

    void init();
    void start();
    void run();
    void kill();

    /// @brief Get the appropriate order associate with the given string
    /// @param s string to convert as an order
    /// @return the associate order or UNKNOWN order
    Application::Command convertToOrder(std::string s);

    /// @brief Evaluate the given input
    /// @param input command to evaluate
    void eval(std::string input);

private:
    static const std::map<std::string, Application::Command> STRING_TO_ORDER;

    std::vector<std::string> buffer;

    void parseInput();

    void Error(std::string e);
    void commandHelp();
    void commandExit();
    void commandClear();
    void commandSave();
    void commandCatalog();
    void commandContacts();
    void commandID();
    void commandSet();
};
