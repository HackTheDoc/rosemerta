#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <map>

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
    std::string getUsername();

    void setName(std::string name);
    std::string getName();

    void setLastname(std::string lastname);
    std::string getLastname();

    void setAge(int age);
    int getAge();

    void setBirthday(std::string birthday);
    std::string getBirthday();

    void setStatus(int s);
    void setStatus(std::string s);
    Identity::Status getStatus();

    void addContact(Contact::Type type, std::string detail);
    std::stack<Contact*>* getContacts();
    void printContacts();

    friend std::ostream& operator<<(std::ostream& stream, const Identity& i);

private:
    static const std::map<Identity::Status, std::string> STATUS_TO_STRING;

    int id;
    std::string username;

    std::string name;
    std::string lastname;

    Date birthday;
    int age;

    Identity::Status status;

    std::stack<Contact*> contacts;
};
