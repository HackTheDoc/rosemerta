#include "include/Identity.h"
#include "include/Application.h"

#include <sstream>
#include <vector>
#include <algorithm>

const std::map<Identity::Status, std::string> Identity::STATUS_TO_STRING = {
    {Identity::Status::UNKNOWN  , "UNKNOWN" },
    {Identity::Status::ALIVE    , "ALIVE"   },
    {Identity::Status::DEAD     , "DEAD"    }
};

Identity::Identity() {
    id = -1;
    username = "unknown";

    name = "unknown";
    lastname = "unknown";

    age = -1;

    status = Identity::Status::UNKNOWN;
}

Identity::~Identity() {
    clear();
}

void Identity::clear()  {
    while(!contacts.empty()) {
        delete contacts.top();
        contacts.pop();
    }
}

void Identity::setID(int id) {
    this->id = id;
}

int Identity::getID() {
    return id;
}

void Identity::setUsername(std::string username) {
    this->username = username;
}

std::string Identity::getUsername() {
    return username;
}

void Identity::setName(std::string name) {
    this->name = name;
}

std::string Identity::getName() {
    return name;
}

void Identity::setLastname(std::string lastname) {
    this->lastname = lastname;
}

std::string Identity::getLastname() {
    return lastname;
}

void Identity::setAge(int age) {
    this->age = age;
}

int Identity::getAge() {
    return age;
}

void Identity::setBirthday(std::string birthday) {
    int d,m,y;
    d = m = y = -1;
    sscanf(birthday.c_str(), "%d-%d-%d", &d,&m,&y);
    this->birthday.setDate(d, m, y);
}

std::string Identity::getBirthday() {
    return birthday.get();
}

void Identity::setStatus(int s) {
    switch (s) {
    case Identity::Status::ALIVE:
        status = Identity::Status::ALIVE;
        break;
    case Identity::Status::DEAD:
        status = Identity::Status::DEAD;
        break;
    default:
        status = Identity::Status::UNKNOWN;
        break;
    }
}

void Identity::setStatus(std::string s) {
    if (s == "alive") status = Identity::Status::ALIVE;
    else if(s == "dead") status = Identity::Status::DEAD;
    else if (s == "unknown") status = Identity::Status::UNKNOWN;
}

Identity::Status Identity::getStatus() {
    return status;
}

void Identity::addContact(Contact::Type t, std::string detail) {
    std::stack<Contact*> temp;
    Contact* c = nullptr;
    while (c == nullptr && !contacts.empty()) {
        if (contacts.top()->getType() == t) c = contacts.top();
        temp.push(contacts.top());
        contacts.pop();
    }
    while(!temp.empty()) {
        contacts.push(temp.top());
        temp.pop();
    }

    if (c == nullptr) {
        c = new Contact(t, detail);
        contacts.push(c);
        return;
    }

    c->addDetails(detail);
}

void Identity::addContact(std::string t, std::string detail) {
    Contact::Type type;
    if (Contact::STRING_TO_TYPE.count(t) == 0) type = Contact::Type::UNKNOWN;
    else type = Contact::STRING_TO_TYPE.at(t);

    addContact(type, detail);
}

std::stack<Contact*>* Identity::getContacts() {
    return &contacts;
}

void Identity::removeContact(std::string t, int index) {
    try {
        std::stack<Contact*> buffer;
        Contact* c = contacts.top();
        for (int i = 0; i < (int)t.size(); i++) {
            if (t[i] == ' ') t[i] = '-';
            else t[i] = tolower(t[i]);
        }
        Contact::Type type = Contact::STRING_TO_TYPE.at(t);
        while (!contacts.empty() && c->getType() != type) {
            buffer.push(c);
            contacts.pop();
            if (!contacts.empty()) contacts.top();
        }

        if (!contacts.empty() && c->removeDetail(index)) {
            contacts.pop();
            delete c;
        }

        while (!buffer.empty()) {
            contacts.push(buffer.top());
            buffer.pop();
        }
    }
    catch(const std::exception& e) {
        Application::Error("unknown or unexistant contact");
    }
    
}

void Identity::printContacts() {
    std::cout << "Contacts of ";
    if (name != "unknown" && lastname != "unknown")
        std::cout << name << " " << lastname;
    else
        std::cout << username;
    std::cout << ":" << std::endl;

    std::stack<Contact*> temp;
    Contact* c;
    while (!contacts.empty()) {
        c = contacts.top();
        contacts.pop();
        temp.push(c);

        std::cout << "  " << *c << std::endl;
    }
    while (!temp.empty()) {
        contacts.push(temp.top());
        temp.pop();
    }
}

std::ostream& operator<<(std::ostream& stream, const Identity& i) {
    stream << "Username : " << i.username << std::endl;
    stream << "Name     : " << i.name << " " << i.lastname << std::endl;
    stream << "Age      : " << i.age << std::endl;
    stream << "Birthday : " << i.birthday << std::endl;
    stream << "Status   : ";
    
    switch (i.status) {
    case Identity::Status::ALIVE:
        stream << "\033[32m";
        break;
    case Identity::Status::DEAD:
        stream << "\033[31m";
        break;
    case Identity::Status::UNKNOWN:
    default:
        stream << "\033[30m";
        break;
    }

    stream << i.STATUS_TO_STRING.at(i.status);
    stream << "\033[0m";
    
    return stream;
}
