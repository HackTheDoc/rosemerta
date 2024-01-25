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

        SIZE,
        LIST,

        NEW,
        DELETE,
        ID,
        GET,
        CONTACTS,

        SET,
        ADD,
        REMOVE,

        EXPORT,
        EXPORT_ALL,
        OUTPUT
    };

    static const std::string version;
    static bool isRunning;

    Application();
    ~Application();

    void start();

    void run();

    void kill();

    /// @brief display a warning
    /// @param w 
    static void Warning(const std::string& w);

    /// @brief display a log
    /// @param l 
    static void Log(const std::string& l);

private:
    static const std::unordered_map<std::string, Command> STRING_TO_COMMAND;

    static Command ConvertToCommand(const std::string& s);

    std::string outputDir;

    /// @brief buffer containing every words of a user input (cleared between every inputs)
    std::vector<std::string> buffer;

    /// @brief evaluate the user input
    /// @param input 
    void eval(const std::string& input);

    /// @brief parse input for user commands
    void parseInput();

    // all the functions associated to a specific command
    void commandHelp();
    void printHelp(const Command c);
    void commandVersion();
    void commandExit();
    void commandClear();

    void commandSize();
    void commandList();

    void commandNew();
    void commandDelete();
    void commandID();
    void commandGet();
    void commandContacts();

    void commandSet();
    void commandAdd();
    void commandRemove();

    void commandExport(int id = -1);
    void commandExportAll();
    void commandOutput();
};
