#include "include/Application.h"
#include "include/Catalog.h"
#include "include/Identity.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

std::string Application::database = "./database.db";
Catalog* Application::catalog = nullptr;
bool Application::isRunning = false;

const std::map<std::string, Application::Command> Application::STRING_TO_ORDER {
    {"unknown"  , Application::Command::UNKNOWN     },
    {"help"     , Application::Command::HELP        },
    {"exit"     , Application::Command::EXIT        },
    {"clear"    , Application::Command::CLEAR       },
    {"save"     , Application::Command::SAVE        },
    {"catalog"  , Application::Command::CATALOG     },
    {"contacts" , Application::Command::CONTACTS    },
    {"id"       , Application::Command::ID          },
    {"set"      , Application::Command::SET         }
};

Application::Application() {}

Application::~Application() {}

void Application::init() {    
    catalog = new Catalog();
    catalog->load();
}

void Application::start() {
    commandClear();

    commandHelp();

    isRunning = true;
}

void Application::run() {
    parseInput();

    eval(buffer.at(0));
}

void Application::kill() {
    catalog->save();
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
        commandHelp();
        break;
    case Application::Command::EXIT:
        commandExit();
        break;
    case Application::Command::CLEAR:
        commandClear();
        break;
    case Application::Command::SAVE:
        commandSave();
        break;
    case Application::Command::CATALOG:
        commandCatalog();
        break;
    case Application::Command::CONTACTS:
        commandContacts();
        break;
    case Application::Command::ID:
        commandID();
        break;
    case Application::Command::SET:
        commandSet();
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
    std::string input;
    while (getline(ss, input, ' ')) {
        buffer.push_back(input);
    }
}

void Application::Error(std::string e) {
    std::cout << "\033[31m";
    std::cout << "ERROR: " << e << std::endl;
    std::cout << "\033[0m";
}

void Application::commandHelp() {
    std::cout << "Usage:"                                                                                   << std::endl;
    std::cout << "  <command> [options]"                                                                    << std::endl;

    std::cout << "Command:"                                                                                     << std::endl;
    std::cout << "  help                        Show this screen"                                               << std::endl;
    std::cout << "  exit                        Quit the application"                                           << std::endl;
    std::cout << "  clear                       Clear the terminal"                                             << std::endl;
    std::cout << "  catalog <  >                Display the entire catalog"                                     << std::endl;
    std::cout << "          <id>                Display a specific identity from the catalog"                   << std::endl;
    std::cout << "  contacts <id>               Display known contacts of a specific registered identity"       << std::endl;
    std::cout << "  id <name>                   Find the id of a registered identity"                           << std::endl;
    std::cout << "  set <id> <param> <value>    Set a new value for a specific param of a specific identity"    << std::endl;
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
    std::cout << "|  ____  _____ ____  ____   ___  _   _    _     |" << std::endl;
    std::cout << "| |  _ \\| ____|  _ \\/ ___| / _ \\| \\ | |  / \\    |" << std::endl;
    std::cout << "| | |_) |  _| | |_) \\___ \\| | | |  \\| | / _ \\   |" << std::endl;
    std::cout << "| |  __/| |___|  _ < ___) | |_| | |\\  |/ ___ \\  |" << std::endl;
    std::cout << "| |_|   |_____|_| \\_\\____/ \\___/|_| \\_/_/   \\_\\ |" << std::endl;
    std::cout << "|_______________________________________________|" << std::endl;
}

void Application::commandSave() {
    if (Catalog::changelog.empty()) {
        std::cout << "nothing to save" << std::endl;
    } else {
        catalog->save();
    }
}

void Application::commandCatalog() {
    if (buffer.size() == 1) {
        std::cout << *catalog << std::endl;
        return;
    }

    int id = stoi(buffer.at(1));
    
    if (catalog->count(id) == 0) {
        Error("unknown id \""+buffer.at(1)+"\""); 
        return;
    }

    std::cout << *catalog->at(id) << std::endl;
}

void Application::commandContacts() {
    if (buffer.size() < 2) {
        Error("missing parameter(s)");
        return;
    }

    int id = stoi(buffer.at(1));
    
    if (catalog->count(id) == 0) {
        Error("unknown id \""+buffer.at(1)+"\""); 
        return;
    }
    
    catalog->at(id)->printContacts();
}

void Application::commandID() {
    if (buffer.size() < 2) {
        Error("missing parameter(s)");
        return;
    }

    std::set<int> ids = catalog->findID(buffer.at(1));

    std::cout << "IDs associate with name " << buffer.at(1) << ":" << std::endl;
    for (const int& id : ids) {
        std::cout << "  " << id << std::endl;
    }
}

void Application::commandSet() {
    // set <id> <param> <value>
    if (buffer.size() < 4) {
        Error("missing parameter(s)");
        std::cout << "set <id> <param> <value>"                         << std::endl;
        std::cout << "          name [string]"                          << std::endl;
        std::cout << "          lastname [string]"                      << std::endl;
        std::cout << "          age [int]"                              << std::endl;
        std::cout << "          birthday [DD-MM-YY] [MM-YY] [DD-MM]"    << std::endl;
        std::cout << "          status [alive] [dead] [unknown]"        << std::endl;
        return;
    }

    int id = stoi(buffer.at(1));
    std::string param = buffer.at(2);
    std::string value = buffer.at(3);

    if (param == "username") {
        catalog->at(id)->setUsername(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangingType::USERNAME));
    }
    else if (param == "name") {
        catalog->at(id)->setName(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangingType::NAME));
    }
    else if (param == "lastname") {
        catalog->at(id)->setLastname(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangingType::LASTNAME));
    }
    else if (param == "age") {
        catalog->at(id)->setAge(stoi(value));
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangingType::AGE));
    }
    else if (param == "birthday") {
        catalog->at(id)->setBirthday(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangingType::BIRTHDAY));
    }
    else if (param == "status") {
        catalog->at(id)->setStatus(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangingType::STATUS));
    }
    else Error("unknown parameter \""+param+"\"");
}
