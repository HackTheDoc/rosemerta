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
    {"unknown"          , Application::Command::UNKNOWN         },
    {"help"             , Application::Command::HELP            },
    {"exit"             , Application::Command::EXIT            },
    {"clear"            , Application::Command::CLEAR           },
    {"save"             , Application::Command::SAVE            },
    {"catalog"          , Application::Command::CATALOG         },
    {"contacts"         , Application::Command::CONTACTS        },
    {"id"               , Application::Command::ID              },
    {"set"              , Application::Command::SET             },
    {"delete"           , Application::Command::DELETE          },
    {"create"           , Application::Command::CREATE          },
    {"add"              , Application::Command::ADD             },
    {"add-contact"      , Application::Command::ADD_CONTACT     },
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
 
    if (buffer.size() > 0) eval(buffer.at(0));
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
    case Application::Command::DELETE:
        commandDelete();
        break;
    case Application::Command::CREATE:
        commandCreate();
        break;
    case Application::Command::ADD:
        commandAdd();
        break;
    case Application::Command::ADD_CONTACT:
        commandAddContact();
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
    std::cout << "ERROR: ";
    std::cout << "\033[0m";
    std::cout << e << std::endl;
}

void Application::Warning(std::string w) {
    std::cout << "\033[33m";
    std::cout << "WARNING: ";
    std::cout << "\033[0m";
    std::cout << w << std::endl;
}

void Application::commandHelp() {
    std::cout << "Usage:"                                                                                   << std::endl;
    std::cout << "  <command> [options]"                                                                    << std::endl;

    std::cout << "Command:"                                                                                     << std::endl;
    std::cout << "  help                        Show this screen"                                               << std::endl;
    std::cout << "  exit                        Quit the application"                                           << std::endl;
    std::cout << "  clear                       Clear the terminal"                                             << std::endl;
    std::cout << "  catalog [id]                Display the catalog or a specific item"                         << std::endl;
    std::cout << "  contacts [id]               Display known contacts of a specific registered identity"       << std::endl;
    std::cout << "  id [name]                   Find the id of a registered identity"                           << std::endl;
    std::cout << "  set [id] [param] [value]    Set a new value for a specific param of a specific identity"    << std::endl;
    std::cout << "  delete id [id]              Delete one's identity"                                          << std::endl;
    std::cout << "         contact [id] [type]  Delete one's specific contact"                                  << std::endl;
    std::cout << "  create                      Create a new identity"                                          << std::endl;
    std::cout << "  add                         Add a data about specific identity"                             << std::endl;
    std::cout << "      contact"                                                                                << std::endl;
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
        std::cout << "contacts <id>" << std::endl;
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

void Application::commandDelete() {
    if (buffer.size() < 2) {
        Error("missing parameter(s)");
        std::cout << "delete id [id]"                       << std::endl;
        std::cout << "delete contact [id] [type] [index]"   << std::endl;
        return;
    }
    if (buffer.at(1) != "id" && buffer.at(1) != "contact") {
        Error("unknown parameter");
        std::cout << "delete id [id]"                       << std::endl;
        std::cout << "delete contact [id] [type] [index]"   << std::endl;
        return;
    }

    if (buffer.at(1) == "id") {
        if (buffer.size() < 3) {
            Error("missing parameter \"id\"");
            std::cout << "delete id [id]" << std::endl;
            return;
        }
        catalog->erase(stoi(buffer.at(2)));
    }
    else {
        if (buffer.size() <= 3) {
            Error("missing parameter(s)");
            std::cout << "delete contact [id] [type] [index=1]"   << std::endl;
            return;
        }
        if (buffer.size() == 4) catalog->eraseContact(stoi(buffer.at(2)), buffer.at(3));
        else catalog->eraseContact(stoi(buffer.at(2)), buffer.at(3), stoi(buffer.at(4)));
    }
}

void Application::commandCreate() {
    // with full args
    /*if (buffer.size() >= 5) {
        std::string details = buffer.at(4);
        for (int i = 5; i < (int)buffer.size(); i++)
            details += "|" + buffer.at(i);

        Identity* id = catalog->at(stoi(buffer.at(2)));
        if (id == nullptr) return;

        id->addContact(buffer.at(3), details);

        Catalog::changelog.insert(std::make_pair(id->getID(), Catalog::ChangingType::NEW_IDENTITY));

        return;
    }*/

    // manual creation
    Warning("use '-1' value to set to unknown");
    std::cout << "  username: ";
    std::string username;
    std::cin >> username;

    std::cout << "  name: ";
    std::string name;
    std::cin >> name;

    std::cout << "  lastname: ";
    std::string lastname;
    std::cin >> lastname;

    std::cout << "  age: ";
    std::string age;
    std::cin >> age;

    std::cout << "  birthday: ";
    std::string birthday;
    std::cin >> birthday;

    std::cout << "  status: ";
    std::string status;
    std::cin >> status;
    /*
    std::cout << "  note: ";
    std::string note;
    std::cin >> note;
    */
    std::cout << "  valid? (y/n) ";
    std::string validation;
    std::cin >> validation;

    if (validation != "y"   &&
        validation != "Y"   &&
        validation != "yes" &&
        validation != "YES" &&
        validation != "Yes"
        ) {
            Warning("aborted");
            return;
        }

    
    Identity* id = new Identity();
    if (username != "-1")   id->setUsername(username);
    if (name != "-1")       id->setName(name);
    if (lastname != "-1")   id->setLastname(lastname);
    if (age != "-1")        id->setAge(stoi(age));
    if (birthday != "-1")   id->setBirthday(birthday);
    if (status != "-1")     id->setStatus(status);
    //if (note != "-1")     id->setNote(note);

    catalog->insert(id);

    Catalog::changelog.insert(std::make_pair(id->getID(), Catalog::ChangingType::NEW_IDENTITY));
    catalog->save();
}

void Application::commandAdd() {
    std::string redirect;
    if (buffer.size() < 2) {
        std::cout << "  type of information (ex:contact): ";
        std::cin >> redirect;
    } else {
        redirect = buffer.at(1);
    }
    
    if (redirect == "contact") commandAddContact();
    else Warning("aborted");
}

void Application::commandAddContact() {
    // with full args
    if (buffer.size() >= 5) {
        std::string details = buffer.at(4);
        for (int i = 5; i < (int)buffer.size(); i++)
            details += "|" + buffer.at(i);

        Identity* id = catalog->at(stoi(buffer.at(2)));
        if (id == nullptr) return;

        id->addContact(buffer.at(3), details);

        Catalog::changelog.insert(std::make_pair(id->getID(), Catalog::ChangingType::UPDATE_CONTACT));

        return;
    }

    // manual creation
    std::cout << "  owner id: ";
    std::string r_owner;
    std::cin >> r_owner;

    std::cout << "  type: ";
    std::string type;
    std::cin >> type;

    std::cout << "  detail: ";
    std::string detail;
    std::cin >> detail;

    if (r_owner == "" || type == "" || detail == "") {
        Error("incorrect(s) input(s)");
        return;
    }

    std::cout << "  valid? (y/n) ";
    std::string validation;
    std::cin >> validation;

    if (validation != "" &&
        validation != "y"   &&
        validation != "Y"   &&
        validation != "yes" &&
        validation != "YES" &&
        validation != "Yes"
        ) {
            Warning("aborted");
            return;
        }

    
    Identity* id = catalog->at(stoi(r_owner));
    if (id == nullptr) return;

    id->addContact(type, detail);

    Catalog::changelog.insert(std::make_pair(id->getID(), Catalog::ChangingType::UPDATE_CONTACT));
}
