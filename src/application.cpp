#include "include/application.h"
#include "include/database.h"

#include <iostream>
#include <pwd.h>
#include <unistd.h>

const std::string Application::version = "1.0";
bool Application::isRunning = false;

const std::unordered_map<std::string, Application::Command> Application::STRING_TO_COMMAND = {
    {"unknown"  , Application::Command::UNKNOWN },
    
    // terminal cmds
    {"help"     , Application::Command::HELP    },

    {"version"  , Application::Command::VERSION },
    {"ver"      , Application::Command::VERSION },

    {"exit"     , Application::Command::EXIT    },
    
    {"clear"    , Application::Command::CLEAR   },
    {"cls"      , Application::Command::CLEAR   },

    // database pragma cmds
    {"size"     , Application::Command::SIZE    },
    {"sz"       , Application::Command::SIZE    },
};

Application::Application() {}

Application::~Application() {}

void Application::start() {
    // init db
    const char* homeDir = std::getenv("HOME");
    if (homeDir) {
        std::string home(homeDir);
        Database::SetPath(home+"/.rosemerta/");
    }
    else
        Database::SetPath("/tmp/rosemerta.db");

    if (!Database::Exist()) Database::Create();

    // init cmds
    commandClear();

    std::cout << "Welcome ";

    /// TODO: username parsing
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);

    if (pw != nullptr) {
        std::cout << pw->pw_name;
    }
    
    std::cout << "! try out ";
    std::cout << "\033[33m" << "help" << "\033[0m";
    std::cout << " command!" << std::endl;
    
    isRunning = true;
}

void Application::run() {
    parseInput();
 
    if (buffer.size() > 0) eval(buffer.at(0));
}

void Application::kill() {
    // nothing to kill for now
}

void Application::Error(const std::string& e) {
    std::cout << "\033[31m";
    std::cout << "ERROR: ";
    std::cout << "\033[0m";
    std::cout << e << std::endl;
}

void Application::Warning(const std::string& w) {
    std::cout << "\033[33m";
    std::cout << "WARNING: ";
    std::cout << "\033[0m";
    std::cout << w << std::endl;
}

void Application::Log(const std::string& l) {
    std::cout << "\033[34m";
    std::cout << "LOG: ";
    std::cout << "\033[0m";
    std::cout << l << std::endl;
}

Application::Command Application::ConvertToCommand(const std::string& s) {
    if (STRING_TO_COMMAND.count(s) <= 0) 
        return Application::Command::UNKNOWN;
    return STRING_TO_COMMAND.at(s);
}

void Application::eval(const std::string& input) {
    const Application::Command c = ConvertToCommand(input);

    switch (c) {
    case Application::Command::HELP:
        commandHelp();
        break;
    case Application::Command::VERSION:
        commandVersion();
        break;
    case Application::Command::EXIT:
        commandExit();
        break;
    case Application::Command::CLEAR:
        commandClear();
        break;
    case Application::Command::SIZE:
        commandSize();
        break;
    case Application::Command::UNKNOWN:
    default:
        Error("command \""+buffer.at(0)+"\" is unkown");
        break;
    }
}

void Application::parseInput() {
    buffer.clear();

    std::cout << "\033[32m";
    std::cout << "> ";
    std::cout << "\033[0m";

    std::string raw;
    std::getline(std::cin, raw);

    std::stringstream ss(raw);
    std::string word;
    bool insideQuote = false;

    while (getline(ss, word, ' ')) {
        if (!insideQuote) {
            if (word.front() == '"') {
                // If an opening quote is found, set the flag and remove the quote character
                insideQuote = true;
                word = word.substr(1);
            }
            buffer.push_back(word);
        }
        else {
            if (word.back() == '"') {
                // If a closing quote is found, reset the flag and remove the quote character
                insideQuote = false;
                word.pop_back();
                buffer.back() += ' ' + word; // Append to the last word in the buffer
            } else {
                // If we are inside a quote, keep appending to the last word in the buffer
                buffer.back() += ' ' + word;
            }
        }
    }
}

/* ----- COMMANDS RELATED FUNCTIONS ----- */
void Application::commandHelp() {
    if (buffer.size() > 1) {
        const Application::Command c = ConvertToCommand(buffer.at(1));

        switch (c) {
        case Application::Command::HELP:
            std::cout << "help              List of commands"                           << std::endl;
            std::cout << "help <command>    Display help of a specific command"         << std::endl;
            break;
        case Application::Command::VERSION:
            std::cout << "version           Get the current version of the application" << std::endl;
            break;
        case Application::Command::EXIT:
            std::cout << "exit              Close the application"                      << std::endl;
            break;
        case Application::Command::CLEAR:
            std::cout << "clear             Clear the terminal"                         << std::endl;
            break;
        case Application::Command::SIZE:
            std::cout << "size              Current number of registered identities"    << std::endl;
            break;
        case Application::Command::UNKNOWN:
        default:
            break;
        }

        if (c != Application::Command::UNKNOWN)
            return;
    }

    std::cout << "Usage:"                       << std::endl;
    std::cout << "  <command> [options]"        << std::endl;

    std::cout << std::endl;

    std::cout << "Command:"                     << std::endl;
    std::cout << "  help"                       << std::endl;
    std::cout << "  help <command>"             << std::endl;
    std::cout << "  version"                    << std::endl;
    std::cout << "  exit"                       << std::endl;
    std::cout << "  clear"                      << std::endl;

    std::cout << std::endl;

    std::cout << "  size"                       << std::endl;
}

void Application::commandVersion() {
    std::cout << "rosemerta " << version << std::endl;
}

void Application::commandExit() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
    
    isRunning = false;
}

void Application::commandClear() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
    
    std::cout << " _______________________________________________" << std::endl;
    std::cout << "|  ____  _____ ____  ____   ___  _   _    _ v1.0|" << std::endl;
    std::cout << "| |  _ \\| ____|  _ \\/ ___| / _ \\| \\ | |  / \\    |" << std::endl;
    std::cout << "| | |_) |  _| | |_) \\___ \\| | | |  \\| | / _ \\   |" << std::endl;
    std::cout << "| |  __/| |___|  _ < ___) | |_| | |\\  |/ ___ \\  |" << std::endl;
    std::cout << "| |_|   |_____|_| \\_\\____/ \\___/|_| \\_/_/   \\_\\ |" << std::endl;
    std::cout << "|_______________________________________________|" << std::endl;
}

void Application::commandSize() {
    std::cout << Database::Size() << std::endl;
}
