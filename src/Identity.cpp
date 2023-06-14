#include "include/Identity.h"

#include <sstream>
#include <vector>

Identity::Identity() {
    id = -1;
    username = "unknown";

    name = "unknown";
    lastname = "unknown";

    age = -1;

    status = Identity::Status::UNKNOWN;
}

Identity::~Identity() {
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

void Identity::setName(std::string name) {
    this->name = name;
}

void Identity::setLastname(std::string lastname) {
    this->lastname = lastname;
}

void Identity::setAge(int age) {
    this->age = age;
}

void Identity::setBirthday(std::string birthday) {
    std::vector<int> buffer;
    std::string s;
    std::stringstream ss(birthday);
    while (getline(ss, s, '-')) {
        buffer.push_back(stoi(s));
    }

    this->birthday.setDate(buffer.at(0), buffer.at(1), buffer.at(2));
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

void Identity::addContact(Contact::Type type, std::string detail) {
    Contact* c = new Contact(type, detail);
    contacts.push(c);
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
    stream << "Status   : " << i.status;
    
    return stream;
}
