#pragma once

#include <iostream>
#include <string>
#include <stack>

#include "Contact.h"
#include "Date.h"

class Identity {
public:
    enum Status {
        UNKNOWN,
        ALIVE,
        DEAD
    };

    Identity();
    ~Identity();

    void setID(int id);
    int getID();

    void setUsername(std::string username);
    void setName(std::string name);
    void setLastname(std::string lastname);
    void setAge(int age);
    void setBirthday(std::string birthday);
    void setStatus(int s);

    void addContact(Contact::Type type, std::string detail);
    void printContacts();

    friend std::ostream& operator<<(std::ostream& stream, const Identity& i);

private:
    int id;
    std::string username;

    std::string name;
    std::string lastname;

    Date birthday;
    int age;

    Identity::Status status;

    std::stack<Contact*> contacts;
};
