#include "include/Application.h"
#include "include/Catalog.h"
#include "include/Identity.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

sqlite3* Application::database = nullptr;
Catalog* Application::catalog = nullptr;
bool Application::isRunning = false;

const std::map<std::string, Application::Command> Application::STRING_TO_ORDER {
    {"unknown"  , Application::Command::UNKNOWN     },
    {"help"     , Application::Command::HELP        },
    {"exit"     , Application::Command::EXIT        },
    {"clear"    , Application::Command::CLEAR       },
    {"catalog"  , Application::Command::CATALOG     },
    {"contacts" , Application::Command::CONTACTS    },
    {"id"       , Application::Command::ID          }
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
    parseInput();

    eval(buffer.at(0));
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
        exit();
        break;
    case Application::Command::CLEAR:
        clear();
        break;
    case Application::Command::CATALOG:
        printCatalog();
        break;
    case Application::Command::CONTACTS:
        printContacts();
        break;
    case Application::Command::ID:
        findID();
        break;
    case Application::Command::UNKNOWN:
    default:
        printError("command \""+buffer.at(0)+"\" is unkown");
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
    std::string input;
    while (getline(ss, input, ' ')) {
        buffer.push_back(input);
    }
}

void Application::printError(std::string e) {
    std::cout << "\033[31m";
    std::cout << "ERROR: " << e << std::endl;
    std::cout << "\033[0m";
}

void Application::printHelper() {
std::cout << "Usage:"                                                                                   << std::endl;
    std::cout << "  <command> [options]"                                                                << std::endl;

    std::cout << "Command:"                                                                             << std::endl;
    std::cout << "  help                Show this screen"                                               << std::endl;
    
    std::cout << "  exit                Quit the application"                                           << std::endl;
    
    std::cout << "  clear               Clear the terminal"                                             << std::endl;
    
    std::cout << "  catalog <  >        Display the entire catalog"                                     << std::endl;
    std::cout << "          <id>        Display a specific identity from the catalog"                   << std::endl;
    
    std::cout << "  contacts <id>       Display known contacts of a specific registered identity"       << std::endl;

    std::cout << "  id <username>       Find the id of a referenced username"                           << std::endl;
    std::cout << "     <name>           Find the id of a referenced name"                               << std::endl;
}

void Application::printCatalog() {
    if (buffer.size() == 1) {
        std::cout << *catalog << std::endl;
        return;
    }

    int id = stoi(buffer.at(1));
    
    if (catalog->count(id) == 0) {
        printError("unknown id \""+buffer.at(1)+"\""); 
        return;
    }

    std::cout << *catalog->at(id) << std::endl;
}

void Application::printContacts() {
    if (buffer.size() < 2) {
        printError("missing parameter");
        return;
    }

    int id = stoi(buffer.at(1));
    
    if (catalog->count(id) == 0) {
        printError("unknown id \""+buffer.at(1)+"\""); 
        return;
    }
    
    catalog->at(id)->printContacts();
}

void Application::findID() {
    if (buffer.size() < 2) {
        printError("missing parameter");
        return;
    }

    std::set<int> ids = catalog->findID(buffer.at(1));

    std::cout << "IDs associate with name " << buffer.at(1) << ":" << std::endl;
    for (const int& id : ids) {
        std::cout << "  " << id << std::endl;
    }
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

void Application::exit() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
    
    isRunning = false;
}
