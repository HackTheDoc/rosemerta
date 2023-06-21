#include "include/Application.h"
#include "include/Catalog.h"
#include "include/Identity.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

std::string Application::database = "./databases/unknown.db";
Catalog* Application::catalog = nullptr;
bool Application::isRunning = false;

const std::map<std::string, Application::Command> Application::STRING_TO_COMMAND {
    {"unknown"          , Application::Command::UNKNOWN         },
    {"help"             , Application::Command::HELP            },
    {"exit"             , Application::Command::EXIT            },
    {"clear"            , Application::Command::CLEAR           },
    {"save"             , Application::Command::SAVE            },
    {"catalog"          , Application::Command::CATALOG         },
    {"contacts"         , Application::Command::CONTACTS        },
    {"id"               , Application::Command::ID              },
    {"set"              , Application::Command::SET             },
    {"create"           , Application::Command::CREATE          },
    {"add"              , Application::Command::ADD             },
    {"add-contact"      , Application::Command::ADD_CONTACT     },
    {"add-note"         , Application::Command::ADD_NOTE        },
    {"delete"           , Application::Command::DELETE          },
    {"remove"           , Application::Command::REMOVE          },
    {"remove-contact"   , Application::Command::REMOVE_CONTACT  },
    {"remove-note"      , Application::Command::REMOVE_NOTE     },
};

Application::Application() {}

Application::~Application() {}

void Application::init(std::string username, bool n) {
    database = "./database/"+username+".db";

    if (n) createNewDatabase();

    catalog = new Catalog();
    catalog->load();
}

void Application::start() {
    commandClear();

    std::cout << "try out ";
    std::cout << "\033[33m" << "help" << "\033[0m";
    std::cout << " command!" << std::endl;

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

Application::Command Application::convertToCommand(std::string s) {
    if (STRING_TO_COMMAND.count(s) <= 0) return Application::Command::UNKNOWN;
    else return STRING_TO_COMMAND.at(s);
}

void Application::eval(std::string input) {
    Application::Command c = convertToCommand(input);

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
    case Application::Command::CREATE:
        commandCreate();
        break;
    case Application::Command::ADD:
        commandAdd();
        break;
    case Application::Command::ADD_CONTACT:
        commandAddContact();
        break;
    case Application::Command::ADD_NOTE:
        commandAddNote();
        break;
    case Application::Command::DELETE:
        commandDelete();
        break;
    case Application::Command::REMOVE:
        commandRemove();
        break;
    case Application::Command::REMOVE_CONTACT:
        commandRemoveContact();
        break;
    case Application::Command::REMOVE_NOTE:
        commandRemoveNote();
        break;
    case Application::Command::UNKNOWN:
    default:
        Error("command \""+buffer.at(0)+"\" is unkown");
        break;
    }
}

void Application::createNewDatabase() {
    sqlite3* db;
    sqlite3_open(database.c_str(), &db);

    std::string identityQuery = "CREATE TABLE IF NOT EXISTS \"identity\" (\"id\" INTEGER UNIQUE, \"username\" TEXT,	\"name\" TEXT, \"lastname\"	TEXT, \"age\" INTEGER, \"birthday\"	TEXT, \"status\" INTEGER, \"notes\"	TEXT, PRIMARY KEY(\"id\"));";
    if (sqlite3_exec(db, identityQuery.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        Error(sqlite3_errmsg(db));
        return;
    }

    std::string contactQuery = "CREATE TABLE IF NOT EXISTS \"contact\" (\"owner\" INTEGER, \"type\" INTEGER, \"detail\" TEXT);";
    if (sqlite3_exec(db, contactQuery.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        Error(sqlite3_errmsg(db));
        return;
    }

    sqlite3_close(db);
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
    std::cout << "  create                      Create a new identity"                                          << std::endl;
    std::cout << "  add [attribute] [id]        Add an attribute to a specific identity"                        << std::endl;
    std::cout << "  delete [id]                 Delete one's identity"                                          << std::endl;
    std::cout << "  remove [attribute] [id]     Remove an attribute of an identity (ex: contact)"               << std::endl;
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
    std::cout << "|  ____  _____ ____  ____   ___  _   _    _ v0.2|" << std::endl;
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
        catalog->display();
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
        std::cout << "          username [string]"                      << std::endl;
        std::cout << "          name [string]"                          << std::endl;
        std::cout << "          lastname [string]"                      << std::endl;
        std::cout << "          age [int]"                              << std::endl;
        std::cout << "          birthday [DD-MM-YY]"                    << std::endl;
        std::cout << "          status [alive] [dead] [unknown]"        << std::endl;
        return;
    }

    int id = stoi(buffer.at(1));
    std::string param = buffer.at(2);
    std::string value = buffer.at(3);

    if (param == "username") {
        catalog->at(id)->setUsername(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangelogType::USERNAME));
    }
    else if (param == "name") {
        catalog->at(id)->setName(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangelogType::NAME));
    }
    else if (param == "lastname") {
        catalog->at(id)->setLastname(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangelogType::LASTNAME));
    }
    else if (param == "age") {
        catalog->at(id)->setAge(stoi(value));
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangelogType::AGE));
    }
    else if (param == "birthday") {
        catalog->at(id)->setBirthday(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangelogType::BIRTHDAY));
    }
    else if (param == "status") {
        catalog->at(id)->setStatus(value);
        Catalog::changelog.insert(std::make_pair(id, Catalog::ChangelogType::STATUS));
    }
    else Error("unknown parameter \""+param+"\"");
}

void Application::commandCreate() {
    // TODO: with full args

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
    std::string r_age;
    std::cin >> r_age;
    int age = 0;
    try {
        age = stoi(r_age);
    }
    catch(const std::exception& e) {}
    

    std::cout << "  birthday: ";
    std::string birthday;
    std::cin >> birthday;

    std::cout << "  status: ";
    std::string status;
    std::cin >> status;
    
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
    id->setAge(age);
    if (birthday != "-1")   id->setBirthday(birthday);
    if (status != "-1")     id->setStatus(status);

    catalog->insert(id);

    Catalog::changelog.insert(std::make_pair(id->getID(), Catalog::ChangelogType::NEW_IDENTITY));
    catalog->save();
}

void Application::commandAdd() {
    std::string redirect;
    if (buffer.size() < 2) {
        std::cout << "  valid types : contact, note" << std::endl;
        std::cout << "  select type of information : ";
        std::cin >> redirect;
    } else {
        redirect = buffer.at(1);
    }
    
    if (redirect == "contact") commandAddContact();
    else if (redirect == "note") commandAddNote();
    else Error("unknown parameter(s)");
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

        Catalog::changelog.insert(std::make_pair(id->getID(), Catalog::ChangelogType::UPDATE_CONTACT));

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

    Catalog::changelog.insert(std::make_pair(id->getID(), Catalog::ChangelogType::UPDATE_CONTACT));
}

void Application::commandAddNote() {
    // with full args
    if (buffer.size() >= 4) {
        Identity* id = catalog->at(stoi(buffer.at(2)));
        if (id == nullptr) return;

        id->addNote(buffer.at(3));

        Catalog::changelog.insert(std::make_pair(id->getID(), Catalog::ChangelogType::NOTES));

        return;
    }

    // manual creation
    Warning("use \"\" if you try to input with spaces");
    std::cout << "  owner id: ";
    std::string r_owner;
    std::cin >> r_owner;

    std::cout << "  note: ";
    std::string note;
    std::cin >> note;

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

    id->addNote(note);

    Catalog::changelog.insert(std::make_pair(id->getID(), Catalog::ChangelogType::NOTES));
}

void Application::commandDelete() {
    if (buffer.size() < 2) {
        Error("missing parameter \"id\"");
        std::cout << "delete [id]" << std::endl;
        return;
    }
    catalog->erase(stoi(buffer.at(1)));
}

void Application::commandRemove() {
    if (buffer.size() < 2) {
        Error("missing parameter");
        std::cout << "remove [attribute] [id] [...]" << std::endl;
        return;
    }
    std::string redirect = buffer.at(1);
    if (redirect == "contact") commandRemoveContact();
    else if (redirect == "note") commandRemoveNote();
    else Error("unknown parameter(s)");
}

void Application::commandRemoveContact() {
    if (buffer.size() == 4  && buffer.at(3) == "all") {
        catalog->removeContacts(stoi(buffer.at(2)));
        return;
    }
    if (buffer.size() < 4) {
        Error("missing parameter(s)");
        std::cout << "delete contact [id] [type] [index=1]"   << std::endl;
        return;
    }
    
    if (buffer.size() == 4) catalog->removeContact(stoi(buffer.at(2)), buffer.at(3));
    else catalog->removeContact(stoi(buffer.at(2)), buffer.at(3), stoi(buffer.at(4)));
}

void Application::commandRemoveNote() {
    if (buffer.size() == 4  && buffer.at(3) == "all") {
        Identity* id = catalog->at(stoi(buffer.at(2)));
        if (id != nullptr) {
            id->removeNotes();
            catalog->changelog.insert(std::make_pair(id->getID(), Catalog::ChangelogType::NOTES));
        }
        return;
    }
    if (buffer.size() < 3) {
        Error("missing parameter(s)");
        std::cout << "delete contact [id] [index=1]"   << std::endl;
        return;
    }
    
    Identity* id = catalog->at(stoi(buffer.at(2)));
    if (id == nullptr) return;
    
    if (buffer.size() == 3) id->removeNote();
    else id->removeNote(stoi(buffer.at(3)));
            
    catalog->changelog.insert(std::make_pair(id->getID(), Catalog::ChangelogType::NOTES));
}
