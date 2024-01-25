#include "include/application.h"
#include "include/database.h"
#include "include/error.h"
#include "include/hue.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include <pwd.h>
#include <unistd.h>

#ifdef _WIN32 // Windows
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#define OPEN_CMD "start "
#else // POSIX (Linux, macOS, etc.)
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0777)
#define OPEN_CMD "open "
#endif

bool isValidDate(const char* date) {
    int y, m, d;
    if (sscanf(date, "%d-%d-%d", &d, &m, &y) == 3 || sscanf(date, "%d/%d/%d", &d, &m, &y) == 3)
        return d >= 1 && d <= 31 && m >= 1 && m <= 12;

    return false;
}

void to_lower(std::string& s) {
    for (char& c : s)
        c = tolower(c);
}

void to_upper(std::string& s) {
    for (char& c : s)
        c = toupper(c);
}

void to_title(std::string& s) {
    if (s.empty()) return;

    for (char& c : s)
        c = tolower(c);

    s[0] = toupper(s[0]);
}

/* ----- APPLICATION CLASS ----- */
const std::string Application::version = "1.1";
bool Application::isRunning = false;

const std::unordered_map<std::string, Application::Command> Application::STRING_TO_COMMAND = {
    {"unknown"      , Command::UNKNOWN                  },

    // terminal cmds                
    {"help"         , Command::HELP                     },

    {"version"      , Command::VERSION                  },
    {"ver"          , Command::VERSION                  },

    {"exit"         , Command::EXIT                     },

    {"clear"        , Command::CLEAR                    },
    {"cls"          , Command::CLEAR                    },

    // database pragma cmds             
    {"size"         , Command::SIZE                     },
    {"sz"           , Command::SIZE                     },

    {"list"         , Command::LIST                     },
    {"ls"           , Command::LIST                     },

    // database manipulation cmds                   
    {"new"          , Command::NEW                      },

    {"delete"       , Command::DELETE                   },
    {"del"          , Command::DELETE                   },

    {"id"           , Command::ID                       },

    {"get"          , Command::GET                      },
    {"addresses"    , Command::ADDRESSES                },
    {"contacts"     , Command::CONTACTS                 },

    {"set"          , Command::SET                      },

    {"add"          , Command::ADD                      },

    {"remove"       , Command::REMOVE                   },
    {"rm"           , Command::REMOVE                   },

    // exportations cmds    
    {"export"       , Application::Command::EXPORT      },

    {"export-all"   , Application::Command::EXPORT_ALL  },
    {"exportall"    , Application::Command::EXPORT_ALL  },
    {"export_all"   , Application::Command::EXPORT_ALL  },

    {"output"       , Application::Command::OUTPUT      }
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

    // init folders
    //// ouput folder
    outputDir = "/tmp/rosemerta-output/";
    if (homeDir) {
        std::string home(homeDir);
        outputDir = home + "/.rosemerta/output/";
    }
    if (!std::filesystem::exists(outputDir)) MKDIR(outputDir.c_str());


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
    case Command::ADDRESSES:
        commandAddresses();
        break;
    case Command::CONTACTS:
        commandContacts();
        break;
    case Command::SET:
        commandSet();
        break;
    case Command::ADD:
        commandAdd();
        break;
    case Command::REMOVE:
        commandRemove();
        break;
    case Command::EXPORT:
        commandExport();
        break;
    case Command::EXPORT_ALL:
        commandExportAll();
        break;
    case Command::OUTPUT:
        commandOutput();
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
    std::cout << "  addresses <id>" << std::endl;
    std::cout << "  contacts <id>" << std::endl;

    std::cout << "  set <id> <param> <value>" << std::endl;
    std::cout << "  add <id> address" << std::endl;
    std::cout << "           contact" << std::endl;
    std::cout << "           note [note]" << std::endl;
    std::cout << "  remove <id> address <title>" << std::endl;
    std::cout << "              contact <type> <index>" << std::endl;
    std::cout << "              note <index>" << std::endl;

    std::cout << std::endl;

    std::cout << "  export <id>" << std::endl;
    std::cout << "  export-all" << std::endl;
    std::cout << "  output" << std::endl;
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
    case Application::Command::ADDRESSES:
        std::cout << "addresses <id>   Get the addresses of an element" << std::endl;
        break;
    case Application::Command::CONTACTS:
        std::cout << "contacts <id>   Get the contacts of an element" << std::endl;
        break;
    case Application::Command::ADD:
        std::cout << "add <id> address      Add an address to of the identiy" << std::endl;
        std::cout << "         contact      Add a contact to the identiy" << std::endl;
        std::cout << "         note <note>  Add a note to the identity (must be in \"\")" << std::endl;
        break;
    case Application::Command::REMOVE:
        std::cout << "remove <id> address <title>           Remove one of the addess of an entity based on it's title" << std::endl;
        std::cout << "            contact <type> <index>    Remove one of the contact of an entity based on it's type and title" << std::endl;
        std::cout << "            note <index>              Remove one of the notes of an entity based on it's index" << std::endl;
        std::cout << "index can be found when you get the identity data (either fully or partially)" << std::endl;
        break;
    case Command::SET:
        std::cout << "set <id> <param> <value>" << std::endl;
        std::cout << "          username [string]                 Set the element's username" << std::endl;
        std::cout << "          firstname [string]                Set the element's firstname" << std::endl;
        std::cout << "          lastname [string]                 Set the element's lastname" << std::endl;
        std::cout << "          age [int]                         Set the element's age" << std::endl;
        std::cout << "          birthday [DD-MM-YY]               Set the element's birthday date" << std::endl;
        std::cout << "          status [alive || dead || unknown] Set wether the element is alive or not" << std::endl;
        std::cout << "          address <title>                   Open a form to modify one of the address of the element" << std::endl;
        break;
    case Application::Command::EXPORT:
        std::cout << "export <id>   Export the profile of an element to an output directory located in the application files" << std::endl;
        std::cout << "              Use 'output' command to open the ouput folder" << std::endl;
        break;
    case Application::Command::EXPORT_ALL:
        std::cout << "export-all    Export every elements of the database" << std::endl;
        break;
    case Application::Command::OUTPUT:
        std::cout << "ouput         Open the ouput directory" << std::endl;
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
    std::cout << "|  ____  _____ ____  ____   ___  _   _    _ v" << version << "|" << std::endl;
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

    std::cout << "  nickname: ";
    std::string nickname;
    std::getline(std::cin, nickname);
    if (nickname == "-1") nickname = "";

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
    to_lower(r_alive);
    if (r_alive == "y" || r_alive == "yes") {
        alive = Entity::Status::ALIVE;
    }
    else if (r_alive == "n" || r_alive == "no") {
        alive = Entity::Status::DEAD;
    }

    std::cout << "  valid? (y/n) ";
    std::string validation;
    std::cin >> validation;
    to_lower(validation);
    if (validation != "y" && validation != "yes") {
        Warning("aborted");
        return;
    }

    int id = Database::Insert(firstname, lastname, nickname);

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
    case Entity::Status::ALIVE:
        hue::println("alive", hue::color::GREEN);
        break;
    case Entity::Status::DEAD:
        hue::println("dead", hue::color::RED);
        break;
    default:
        hue::println("unknown", hue::color::LIGHT_GRAY);
        break;
    }

    if (!p.contacts.empty()) {
        std::cout << "  contacts:" << std::endl;

        Contact::Type currentType = Contact::Type::INVALID;
        int index = 1;
        for (unsigned int i = 0; i < p.contacts.size(); i++) {
            auto c = p.contacts[i];

            if (c.t != currentType) {
                currentType = c.t;
                index = 1;
                std::cout << "  - " << to_string(c.t) << ":" << std::endl;
            }

            std::cout << "    " << index << ". " << to_string(c) << std::endl;

            index++;
        }
    }

    if (!p.addresses.empty()) {
        std::cout << "  address :"<< std::endl;
        for (const Address& a : p.addresses)
            std::cout << "    " << a.name << " : " << a.detail << std::endl;
    }

    if (!p.notes.empty()) {
        std::cout << "  notes   :" << std::endl;;
        for (unsigned int i = 0; i < p.notes.size(); i++)
            std::cout << "    " << i + 1 << ". " << p.notes[i] << std::endl;
    }

}

void Application::commandAddresses() {
    if (buffer.size() < 2) {
        error::raise(error::code::MISSING_PARAMETER, "id");
        printHelp(Command::ADDRESSES);
        return;
    }

    int id = -1;
    try {
        id = stoi(buffer.at(1));
    }
    catch (const std::exception& e) {
        error::raise(error::INVALID_ID);
        return;
    }

    std::vector<Address> addresses = Database::GetAddresses(id);

    if (addresses.empty())
        std::cout << "no registered address for element " << id << std::endl;
    else {
        std::cout << "addresses of " << id << ":" << std::endl;
        for (const Address& a : addresses)
            std::cout << "  " << a.name << " : " << a.detail << std::endl;
    }

}

void Application::commandContacts() {
    if (buffer.size() < 2) {
        error::raise(error::code::MISSING_PARAMETER, "id");
        printHelp(Command::CONTACTS);
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

    std::vector<Contact> contacts = Database::GetContacts(id);

    if (contacts.empty())
        std::cout << "no registered contact for element " << id << std::endl;
    else {
        std::cout << "contacts of " << id << ":" << std::endl;
        Contact::Type currentType = Contact::Type::INVALID;
        int index = 1;
        for (unsigned int i = 0; i < contacts.size(); i++) {
            auto c = contacts[i];

            if (c.t != currentType) {
                currentType = c.t;
                index = 1;
                std::cout << "  - " << to_string(c.t) << ":" << std::endl;
            }

            std::cout << "    " << index << ". " << to_string(c) << std::endl;

            index++;
        }
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

    if (param == "nickname" || param == "nname")
        if (value == "-1") Database::SetNickname(id, "");
        else Database::SetNickname(id, value);
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

        if (value == "alive" || value == "true" || value == "t")
            s = Entity::Status::ALIVE;
        else if (value == "dead" || value == "false" || value == "f")
            s = Entity::Status::DEAD;
        else if (value == "unknown" || value == "-1" || value == "u")
            s = Entity::Status::UNKNOWN;
        else {
            Warning("unknown value \"" + value + "\"");
            return;
        }

        Database::SetStatus(id, s);
    }
    else if (param == "address") {
        if (!Database::ExistAddress(id, value)) {
            error::raise("unknown address title");
            return;
        }

        std::cout << "Address form:" << std::endl;

        std::cout << "  country: ";
        std::string country;
        std::getline(std::cin, country);
        to_upper(country);

        std::cout << "  region: ";
        std::string region;
        std::getline(std::cin, region);
        to_title(region);

        std::cout << "  ZIP code: ";
        std::string zipCode;
        std::getline(std::cin, zipCode);
        for (unsigned int i = 0; i < zipCode.size(); i++)
            if (!isdigit(zipCode[i])) {
                zipCode = "";
                break;
            }

        std::cout << "  city: ";
        std::string city;
        std::getline(std::cin, city);
        to_title(city);

        std::cout << "  street: ";
        std::string street;
        std::getline(std::cin, street);
        to_lower(street);

        std::cout << "  number: ";
        std::string number;
        std::getline(std::cin, number);

        std::cout << "  valid? (y/n) ";
        std::string validation;
        std::cin >> validation;
        to_lower(validation);
        if (validation != "y" && validation != "yes") {
            Warning("aborted");
            return;
        }

        std::string address = number + " " + street + ", " + zipCode + " " + city + " " + region + ", " + country;

        if (address.size() <= 7) {
            error::raise("invalid address input");
            return;
        }

        Database::SetAddress(id, value, address);
    }
    else error::raise(("unknown parameter: " + param).c_str());
}

void Application::commandAdd() {
    if (buffer.size() < 3) {
        error::raise(error::code::MISSING_PARAMETER);
        printHelp(Command::ADD);
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

    if (param == "address") {
        std::cout << "Address form:" << std::endl;

        std::cout << "  title: ";
        std::string title;
        std::getline(std::cin, title);
        to_lower(title);

        std::cout << "  country: ";
        std::string country;
        std::getline(std::cin, country);
        to_upper(country);

        std::cout << "  region: ";
        std::string region;
        std::getline(std::cin, region);
        to_title(region);

        std::cout << "  ZIP code: ";
        std::string zipCode;
        std::getline(std::cin, zipCode);
        for (unsigned int i = 0; i < zipCode.size(); i++)
            if (!isdigit(zipCode[i])) {
                zipCode = "";
                break;
            }

        std::cout << "  city: ";
        std::string city;
        std::getline(std::cin, city);
        to_title(city);

        std::cout << "  street: ";
        std::string street;
        std::getline(std::cin, street);
        to_lower(street);

        std::cout << "  number: ";
        std::string number;
        std::getline(std::cin, number);

        std::cout << "  valid? (y/n) ";
        std::string validation;
        std::cin >> validation;
        to_lower(validation);
        if (validation != "y" && validation != "yes") {
            Warning("aborted");
            return;
        }

        std::string address = number + " " + street + ", " + zipCode + " " + city + " " + region + ", " + country;

        if (title.empty() || address.size() <= 7) {
            error::raise("invalid address input");
            return;
        }
        Database::AddAddress(id, title, address);
    }
    else if (param == "contact") {
        Contact::Type t;
        std::string u, p;
        // manual parsing of the contact
        if (buffer.size() < 4) {
            std::cout << "Contact form:" << std::endl;
            do {
                std::cout << "  type: ";
                std::string rawt;
                std::getline(std::cin, rawt);
                t = to_contact_type(rawt);
                if (t == Contact::Type::INVALID) {
                    Warning("");
                    std::cout << "contact must be one of: "
                        << "unknown, "
                        << "email, "
                        << "phone number, "
                        << "discord, "
                        << "instagram, "
                        << "snapchat, "
                        << "twitter, "
                        << "telegram, "
                        << "bereal"
                        << std::endl;
                }
            } while (t == Contact::Type::INVALID);

            std::cout << "  username: ";
            std::getline(std::cin, u);

            std::cout << "  password: ";
            std::getline(std::cin, p);

            std::cout << "  valid? (y/n) ";
            std::string validation;
            std::cin >> validation;
            to_lower(validation);
            if (validation != "y" && validation != "yes") {
                Warning("aborted");
                return;
            }
        }

        // automatic parsing
        else {
            t = to_contact_type(buffer.at(3));

            u = buffer.at(4);

            p = "";
            if (buffer.size() > 5) {
                p = buffer.at(5);
            }
        }

        Database::AddContact(id, t, u, p);
    }
    else if (param == "note") {
        std::string value;
        if (buffer.size() > 3)
            value = buffer.at(3);
        else {
            std::cout << "please enter the note :" << std::endl;
            std::getline(std::cin, value);
        }
        Database::AddNote(id, value);
    }
    else error::raise(error::code::UNKNOWN_PARAMETER, param.c_str());
}

void Application::commandRemove() {
    if (buffer.size() < 4) {
        error::raise(error::code::MISSING_PARAMETER);
        printHelp(Command::REMOVE);
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

    if (param == "address") {
        std::string title = buffer.at(3);

        std::string address = Database::RemoveAddress(id, title);
        if (address.empty()) {
            error::raise(("unable to remove address with title \"" + buffer.at(3) + "\"").c_str());
        }
        else {
            std::cout << "sucessfully removed \"" << title << "\" address:" << std::endl;
            std::cout << address << std::endl;
        }
    }
    else if (param == "contact") {
        if (buffer.size() < 5) {
            error::raise(error::code::MISSING_PARAMETER);
            std::cout << "remove <id> contact <type> <index>" << std::endl;
            return;
        }

        Contact::Type t = to_contact_type(buffer.at(3));

        int index;
        try {
            index = stoi(buffer.at(4));
        }
        catch (const std::exception& e) {
            error::raise("invalid contact index");
            return;
        }

        Contact contact = Database::RemoveContact(id, t, index);
        std::cout << "removed " << to_string(contact.t) << " contact:" << std::endl;
        std::cout << to_string(contact) << std::endl;
    }
    else if (param == "note") {
        int index;
        try {
            index = stoi(buffer.at(3));
        }
        catch (const std::exception& e) {
            error::raise("invalid note index");
            return;
        }

        std::string note = Database::RemoveNote(id, index);
        if (note.empty()) {
            error::raise(("unable to remove note indexed at " + buffer.at(3)).c_str());
        }
        else {
            std::cout << "sucessfully removed note:" << std::endl;
            std::cout << note << std::endl;
        }
    }
    else error::raise(error::code::UNKNOWN_PARAMETER, param.c_str());
}

void Application::commandExport(int id) {
    bool successMessage = false;

    if (id == -1) {
        successMessage = true;

        if (buffer.size() == 1) {
            error::raise(error::code::MISSING_PARAMETER, "id");
            printHelp(Command::EXPORT);
            return;
        }

        std::string _id = buffer.at(1);
        try {
            id = stoi(_id);
        }
        catch (const std::exception& e) {
            error::raise(error::code::INVALID_ID);
            return;
        }
    }

    Entity e = Database::Get(id);

    if (Database::error != error::code::NONE) {
        error::raise(Database::error);
        return;
    }

    json data = {
        {"firstname", e.firstname},
        {"lastname", e.lastname},
        {"username", e.username},
        {"age", e.age},
        {"birthday", e.birthday},
        {"status", e.status},
        {"notes", e.notes}
    };

    std::string filepath = outputDir + std::to_string(id) + " " + e.formatted_name + ".json";
    for (unsigned int i = 0; i < filepath.size(); i++)
        if (filepath[i] == ' ') filepath[i] = '_';
        else filepath[i] = tolower(filepath[i]);

    std::ofstream outfile(filepath);
    outfile << std::setw(2) << data << std::endl;
    outfile.close();

    if (successMessage)
        std::cout << "successfully exported element " << id << std::endl;
}

void Application::commandExportAll() {
    std::vector<int> ids = Database::ListIDs();

    for (int id : ids) commandExport(id);

    std::cout << "successfully exported all elements!" << std::endl;
}

void Application::commandOutput() {
    int rc = std::system((OPEN_CMD + outputDir).c_str());
    if (rc != 0)
        error::raise(error::code::CANNOT_OPEN_FOLDER);
}
