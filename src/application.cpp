#include "include/application.h"
#include "include/database.h"
#include "include/error.h"
#include "include/hue.h"

#include <iostream>

#include <pwd.h>
#include <unistd.h>

bool isValidDate(const char* date) {
    int y, m, d;
    if (sscanf(date, "%d-%d-%d", &d, &m, &y) == 3 || sscanf(date, "%d/%d/%d", &d, &m, &y) == 3)
        return d >= 1 && d <= 31 && m >= 1 && m <= 12;

    return false;
}

const std::string Application::version = "1.0";
bool Application::isRunning = false;

const std::unordered_map<std::string, Application::Command> Application::STRING_TO_COMMAND = {
    {"unknown"  , Command::UNKNOWN         },

    // terminal cmds
    {"help"     , Command::HELP            },

    {"version"  , Command::VERSION         },
    {"ver"      , Command::VERSION         },

    {"exit"     , Command::EXIT            },

    {"clear"    , Command::CLEAR           },
    {"cls"      , Command::CLEAR           },

    // database pragma cmds
    {"size"     , Command::SIZE            },
    {"sz"       , Command::SIZE            },

    {"list"     , Command::LIST            },
    {"ls"       , Command::LIST            },

    // database manipulation cmds   
    {"new"          , Command::NEW         },

    {"delete"       , Command::DELETE      },
    {"del"          , Command::DELETE      },

    {"id"           , Command::ID          },

    {"get"          , Command::GET         },

    {"set"          , Command::SET         },
};

Application::Application() {}

Application::~Application() {}

void Application::start() {
    // init db
    const char* homeDir = std::getenv("HOME");
    if (homeDir) {
        std::string home(homeDir);
        Database::SetPath(home + "/.rosemerta/");
    }
    else
        Database::SetPath("/tmp/rosemerta.db");

    if (!Database::Exist()) Database::Create();

    // init cmds
    commandClear();

    std::cout << "Welcome ";

    /// TODO: username parsing
    uid_t uid = geteuid();
    struct passwd* pw = getpwuid(uid);

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

void Application::Warning(const std::string& w) {
    hue::print("WARNING: ", hue::color::ORANGE);
    std::cout << w << std::endl;
}

void Application::Log(const std::string& l) {
    hue::print("LOG: ", hue::color::BLUE);
    std::cout << l << std::endl;
}

Application::Command Application::ConvertToCommand(const std::string& s) {
    if (STRING_TO_COMMAND.count(s) <= 0)
        return Command::UNKNOWN;
    return STRING_TO_COMMAND.at(s);
}

void Application::eval(const std::string& input) {
    const Command c = ConvertToCommand(input);

    switch (c) {
    case Command::HELP:
        commandHelp();
        break;
    case Command::VERSION:
        commandVersion();
        break;
    case Command::EXIT:
        commandExit();
        break;
    case Command::CLEAR:
        commandClear();
        break;
    case Command::SIZE:
        commandSize();
        break;
    case Command::LIST:
        commandList();
        break;
    case Command::NEW:
        commandNew();
        break;
    case Command::DELETE:
        commandDelete();
        break;
    case Command::ID:
        commandID();
        break;
    case Command::GET:
        commandGet();
        break;
    case Command::SET:
        commandSet();
        break;
    case Command::UNKNOWN:
    default:
        error::raise(error::code::UNKNOWN_COMMAND, buffer.at(0).c_str());
        break;
    }
}

void Application::parseInput() {
    buffer.clear();

    hue::print("> ", hue::color::GREEN);

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
            }
            else {
                // If we are inside a quote, keep appending to the last word in the buffer
                buffer.back() += ' ' + word;
            }
        }
    }
}

/* ----- COMMANDS RELATED FUNCTIONS ----- */
void Application::commandHelp() {
    if (buffer.size() > 1) {
        const Command c = ConvertToCommand(buffer.at(1));

        printHelp(c);

        if (c != Command::UNKNOWN)
            return;
    }

    std::cout << "Usage:" << std::endl;
    std::cout << "  <command> [options]" << std::endl;

    std::cout << std::endl;

    std::cout << "Command:" << std::endl;
    std::cout << "  help" << std::endl;
    std::cout << "  help <command>" << std::endl;
    std::cout << "  version" << std::endl;
    std::cout << "  exit" << std::endl;
    std::cout << "  clear" << std::endl;

    std::cout << std::endl;

    std::cout << "  size" << std::endl;
    std::cout << "  list [page]" << std::endl;

    std::cout << std::endl;

    std::cout << "  new" << std::endl;
    std::cout << "  delete <id>" << std::endl;
    std::cout << "  id <name>" << std::endl;
    std::cout << "  get <id>" << std::endl;

    std::cout << "  set <id> <param> <value>" << std::endl;
}

void Application::printHelp(const Command c) {
    switch (c) {
    case Command::HELP:
        std::cout << "help              List of commands" << std::endl;
        std::cout << "help <command>    Display help of a specific command" << std::endl;
        break;
    case Command::VERSION:
        std::cout << "version           Get the current version of the application" << std::endl;
        break;
    case Command::EXIT:
        std::cout << "exit              Close the application" << std::endl;
        break;
    case Command::CLEAR:
        std::cout << "clear             Clear the terminal" << std::endl;
        break;
    case Command::SIZE:
        std::cout << "size              Current number of registered identities" << std::endl;
        break;
    case Command::LIST:
        std::cout << "list <page>       List a resume of the element on a specific page (default: page 0)" << std::endl;
    case Command::NEW:
        std::cout << "new           Creation proccess for a new element" << std::endl;
        break;
    case Command::DELETE:
        std::cout << "delete <id>   Delete an existing element from the database based on its id" << std::endl;
        break;
    case Command::ID:
        std::cout << "id <name>     Get the id of an element based on its name or username" << std::endl;
        break;
    case Command::GET:
        std::cout << "get <id>      Get an element based on its id" << std::endl;
        break;
    case Command::SET:
        std::cout << "set <id> <param> <value>" << std::endl;
        std::cout << "          username [string]                 Set the element's username" << std::endl;
        std::cout << "          firstname [string]                Set the element's firstname" << std::endl;
        std::cout << "          lastname [string]                 Set the element's lastname" << std::endl;
        std::cout << "          age [int]                         Set the element's age" << std::endl;
        std::cout << "          birthday [DD-MM-YY]               Set the element's birthday date" << std::endl;
        std::cout << "          status [alive || dead || unknown] Set wether the element is alive or not" << std::endl;
        break;
    case Command::UNKNOWN:
    default:
        break;
    }
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

void Application::commandList() {
    int page = 0;

    if (buffer.size() > 1) {
        try {
            page = stoi(buffer.at(1));
        }
        catch (const std::exception& e) {}
    }

    std::vector<std::pair<int, std::string>> buffer = Database::List(page);

    std::cout << "page " << page << ":" << std::endl;

    for (auto pair : buffer)
        std::cout << "  " << pair.first << ". " << pair.second << std::endl;
}

void Application::commandNew() {
    Warning("use '-1' value to set to unknown");

    std::cout << "  username: ";
    std::string username;
    std::getline(std::cin, username);
    if (username == "-1") username = "";

    std::cout << "  firstname: ";
    std::string firstname;
    std::getline(std::cin, firstname);
    if (firstname == "-1") firstname = "";

    std::cout << "  lastname: ";
    std::string lastname;
    std::getline(std::cin, lastname);
    if (lastname == "-1") lastname = "";

    int age = 0;
    std::cout << "  age: ";
    std::string r_age;
    std::getline(std::cin, r_age);
    try {
        age = stoi(r_age);
    }
    catch (const std::exception& e) {
        age = -1;
    }
    if (age < 0) age = -1;

    std::cout << "  birthday (dd-mm-yyyy): ";
    std::string birthday;
    std::getline(std::cin, birthday);
    if (!isValidDate(birthday.c_str())) birthday = "";

    std::cout << "  alive (y/n): ";
    std::string r_alive;
    std::getline(std::cin, r_alive);
    Entity::Status alive = Entity::Status::UNKNOWN;
    for (int i = 0; i < (int)r_alive.size(); i++)
        r_alive[i] = tolower(r_alive[i]);
    if (r_alive == "y" || r_alive == "yes") {
        alive = Entity::Status::ALIVE;
    }
    else if (r_alive == "n" || r_alive == "no") {
        alive = Entity::Status::DEAD;
    }

    std::cout << "  valid? (y/n) ";
    std::string validation;
    std::cin >> validation;
    for (int i = 0; i < (int)validation.size(); i++)
        validation[i] = tolower(validation[i]);
    if (validation != "y" && validation != "yes") {
        Warning("aborted");
        return;
    }

    int id = Database::Insert(firstname, lastname, username);

    if (id == -1) {
        error::raise(Database::error);
        return;
    }

    Database::SetAge(id, age);
    Database::SetBirthday(id, birthday);
    Database::SetStatus(id, alive);

    std::cout << "entity at id " << id << std::endl;
}

void Application::commandDelete() {
    if (buffer.size() == 1) {
        error::raise(error::code::MISSING_PARAMETER, "id");
        printHelp(Command::DELETE);
        return;
    }

    for (int i = 1; i < (int)buffer.size(); i++) {
        int id;
        try {
            id = stoi(buffer.at(i));
        }
        catch (const std::exception& e) {
            error::raise(error::code::INVALID_ID);
            return;
        }

        bool success = Database::Delete(id);

        if (!success) {
            error::raise(("cannot delete identity " + buffer.at(i)).c_str());
        }
        else {
            std::cout << "successfully deleted identity " << buffer.at(i) << std::endl;
        }
    }
}

void Application::commandID() {
    if (buffer.size() < 2) {
        error::raise(error::code::MISSING_PARAMETER); 
        printHelp(Command::ID);
        return;
    }

    std::set<int> ids = Database::Find(buffer.at(1));

    if (ids.empty()) {
        std::cout << "no id found" << std::endl;
        return;
    }

    std::cout << "IDs associate with \"" << buffer.at(1) << "\":" << std::endl;
    for (const int& id : ids) {
        std::cout << "  " << id << std::endl;
    }
}

void Application::commandGet() {
    if (buffer.size() == 1) {
        error::raise(error::code::MISSING_PARAMETER, "id");
        printHelp(Command::GET);
        return;
    }

    std::string idt = buffer.at(1);
    int id;
    try {
        id = stoi(idt);
    }
    catch (const std::exception& e) {
        error::raise(error::code::INVALID_ID);
        return;
    }

    Entity p = Database::Get(id);

    if (Database::error != error::code::NONE) {
        error::raise(Database::error);
        return;
    }

    std::cout << idt << ". ";

    if (p.formatted_name.empty())
        hue::println("Unknown", hue::color::LIGHT_GRAY);
    else
        hue::println(p.formatted_name.c_str());

    if (!p.username.empty())
        std::cout << "  username: " << p.username << std::endl;

    if (p.age != -1)
        std::cout << "  age     : " << p.age << std::endl;

    if (!p.birthday.empty())
        std::cout << "  birthday: " << p.birthday << std::endl;

    std::cout << "  status  : ";
    switch (p.status) {
    case true:
        hue::println("alive", hue::color::GREEN);
        break;
    case false:
        hue::println("dead", hue::color::RED);
        break;
    default:
        hue::println("unknown", hue::color::LIGHT_GRAY);
        break;
    }
}

void Application::commandSet() {
    if (buffer.size() < 4) {
        error::raise(error::code::MISSING_PARAMETER);
        printHelp(Command::SET);
        return;
    }

    int id = -1;
    try {
        id = stoi(buffer.at(1));
    }
    catch (const std::exception& e) {
        error::raise(error::code::INVALID_ID);
        return;
    }
    std::string param = buffer.at(2);
    std::string value = buffer.at(3);

    if (param == "username" || param == "uname")
        if (value == "-1") Database::SetUsername(id, "");
        else Database::SetUsername(id, value);
    else if (param == "firstname" || param == "fname")
        if (value == "-1") Database::SetFirstname(id, "");
        else Database::SetFirstname(id, value);
    else if (param == "lastname" || param == "lname")
        if (value == "-1") Database::SetLastname(id, "");
        else Database::SetLastname(id, value);
    else if (param == "age") {
        try {
            Database::SetAge(id, stoi(value));
        }
        catch (const std::exception& e) {
            Warning("please enter a valid age number");
        }
    }
    else if (param == "birthday" || param == "bday") {
        if (isValidDate(value.c_str()))
            Database::SetBirthday(id, value);
        else
            Warning("invaling date format (dd-mm-yyyy)");
    }
    else if (param == "status") {
        Entity::Status s = Entity::Status::UNKNOWN;
        int v;
        try {
            v = stoi(value);
        }
        catch (const std::exception& e) {
            v = -1;
            return;
        }

        if (value == "alive" || v == true) s = Entity::Status::ALIVE;
        else if (value == "dead" || v == false) s = Entity::Status::DEAD;
        else if (value == "unknown" || v == -1) s = Entity::Status::UNKNOWN;
        else {
            Warning("unknown value \"" + value + "\"");
            return;
        }

        Database::SetStatus(id, s);
    }
    else error::raise(("unknown parameter: " + param).c_str());
}
