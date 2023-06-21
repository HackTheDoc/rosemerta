#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>

class Catalog;

/// @brief A class representing Persona application
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

    /// @brief Initialize the application with the user's username
    /// @param username the user name
    /// @param n true if it is a new user, else false
    void init(std::string username, bool n);
    /// @brief start running the application
    void start();
    /// @brief keep it running
    void run();
    /// @brief kill the app after closing it
    void kill();

    /// @brief Get the appropriate order associate with the given string
    /// @param s string to convert as an order
    /// @return the associate order or UNKNOWN order
    Application::Command convertToCommand(std::string s);

    /// @brief Evaluate the given input
    /// @param input command to evaluate
    void eval(std::string input);
    
    /// @brief display an error
    /// @param e error message to display
    static void Error(std::string e);
    /// @brief display a warning
    /// @param w warning message to display
    static void Warning(std::string w);

private:
    /// @brief converter map from a string to its associated command
    static const std::map<std::string, Application::Command> STRING_TO_COMMAND;

    /// @brief buffer containing every words of a user input (cleared between every inputs)
    std::vector<std::string> buffer;

    /// @brief create a new empty database with the needed table and named 'database-{username}.db'
    void createNewDatabase();

    /// @brief parse input for user commands
    void parseInput();

    // all the functions associated to a specific command
    
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
