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
        SET,
        CREATE,
        ADD,
        ADD_CONTACT,
        ADD_NOTE,
        DELETE,
        REMOVE,
        REMOVE_CONTACT,
        REMOVE_NOTE
    };

    static std::string database;
    static Catalog* catalog;
    static bool isRunning;

    Application();
    ~Application();

    void init(std::string username, bool n);
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
    
    static void Error(std::string e);
    static void Warning(std::string w);

private:
    static const std::map<std::string, Application::Command> STRING_TO_ORDER;

    std::vector<std::string> buffer;

    void createNewDatabase();

    void parseInput();

    void commandHelp();
    void commandExit();
    void commandClear();
    void commandSave();
    void commandCatalog();
    void commandContacts();
    void commandID();
    void commandSet();
    void commandCreate();
    void commandAdd();
    void commandAddContact();
    void commandAddNote();
    void commandDelete();
    void commandRemove();
    void commandRemoveContact();
    void commandRemoveNote();
};
