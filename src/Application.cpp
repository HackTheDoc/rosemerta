#include "include/Application.h"
#include "include/Catalog.h"

#include <iostream>
#include <cstdlib>

sqlite3* Application::database = nullptr;
Catalog* Application::catalog = nullptr;
bool Application::isRunning = false;

const std::map<std::string, Application::Command> Application::STRING_TO_ORDER {
    {"unknown"  , Application::Command::UNKNOWN     },
    {"help"     , Application::Command::HELP        },
    {"exit"     , Application::Command::EXIT        },
    {"clear"    , Application::Command::CLEAR       },
    {"catalog"  , Application::Command::CATALOG     }
};

Application::Application() {}

Application::~Application() {}

void Application::init() {
    sqlite3_open("./database.db", &database);
    
    catalog = new Catalog();
    catalog->load();
}

void Application::start() {
    clear();

    printHelper();

    isRunning = true;
}

void Application::run() {
    std::cout << "\n> ";

    std::string input;
    std::cin >> input;

    eval(input);
}

void Application::kill() {
    sqlite3_close(database);
    database = nullptr;

    catalog->clear();
    delete catalog;
    catalog = nullptr;
}

Application::Command Application::convertToOrder(std::string s) {
    if (STRING_TO_ORDER.count(s) <= 0) return Application::Command::UNKNOWN;
    else return STRING_TO_ORDER.at(s);
}

void Application::eval(std::string input) {
    Application::Command c = convertToOrder(input);

    switch (c) {
    case Application::Command::HELP:
        printHelper();
        break;
    case Application::Command::EXIT:
        isRunning = false;
        break;
    case Application::Command::CLEAR:
        clear();
        break;
    case Application::Command::CATALOG:
        printCatalog();
        break;
    case Application::Command::UNKNOWN:
    default:
        unknownCommandError(input);
        break;
    }
}

void Application::unknownCommandError(std::string c) {
    std::cout << "ERROR: command \"" << c << "\" is unknown" << std::endl;
}

void Application::printHelper() {
    std::cout << "Usage:"                               << std::endl;
    std::cout << "  <command> [options]"                << std::endl;
    
    std::cout << "Command:"                             << std::endl;
    std::cout << "  help        Show this screen"       << std::endl;
    std::cout << "  exit        Quit the application"   << std::endl;
    std::cout << "  catalog     Display catalog"        << std::endl;
}

void Application::printCatalog() {
    std::cout << *catalog << std::endl;
}

void Application::clear() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
    
    std::cout << " _______________________________________________" << std::endl;
    std::cout << "|  ____  _____ ____  ____   ___  _   _    _     |" << std::endl;
    std::cout << "| |  _ \\| ____|  _ \\/ ___| / _ \\| \\ | |  / \\    |" << std::endl;
    std::cout << "| | |_) |  _| | |_) \\___ \\| | | |  \\| | / _ \\   |" << std::endl;
    std::cout << "| |  __/| |___|  _ < ___) | |_| | |\\  |/ ___ \\  |" << std::endl;
    std::cout << "| |_|   |_____|_| \\_\\____/ \\___/|_| \\_/_/   \\_\\ |" << std::endl;
    std::cout << "|_______________________________________________|" << std::endl;
}
