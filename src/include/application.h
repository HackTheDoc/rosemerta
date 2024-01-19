#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Application {
public:
    enum Command {
        UNKNOWN,

        HELP,
        VERSION,

        EXIT,
        CLEAR,

        SAVE,
        SIZE,

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

    static const std::string version;
    static bool isRunning;

    Application();
    ~Application();

    void start();

    void run();

    void kill();

    /// @brief display an error
    /// @param e 
    static void Error(const std::string& e);

    /// @brief display a warning
    /// @param w 
    static void Warning(const std::string& w);

    /// @brief display a log
    /// @param l 
    static void Log(const std::string& l);

private:
    static const std::unordered_map<std::string, Command> STRING_TO_COMMAND;

    static Command ConvertToCommand(const std::string& s);

    /// @brief buffer containing every words of a user input (cleared between every inputs)
    std::vector<std::string> buffer;

    /// @brief evaluate the user input
    /// @param input 
    void eval(const std::string& input);

    /// @brief parse input for user commands
    void parseInput();

    // all the functions associated to a specific command
    void commandHelp();
    void commandVersion();
    void commandExit();
    void commandClear();

    void commandSize();
};
